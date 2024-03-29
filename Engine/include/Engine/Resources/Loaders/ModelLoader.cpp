#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <string>
#include <sstream>
#include "ModelLoader.h"
#include "Engine/Resources/Mesh.h"
#include "Engine/Resources/Model.h"

namespace Eng {

    void ProcessNode(aiNode* node, const aiScene* scene, Model& model, const std::string& file_directory);
    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene, const std::string& file_directory);
    void LoadTexture(aiMaterial* mat, aiTextureType texture_type, Texture2DUsage engine_usage,
                     const std::string& file_directory, std::vector<Texture2DMeshHandle>& textures);

    void ModelLoader::LoadModel(const std::string& path, Model& model )
	{
        Assimp::Importer import;
        const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace  );

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
			std::stringstream error_message;
			error_message << "RESOURCE LOAD ERROR" << import.GetErrorString() << "\n";
            throw std::runtime_error(error_message.str());
        }
        std::string file_directory = path.substr(0, path.find_last_of('/'));
        ProcessNode(scene->mRootNode, scene, model, file_directory);
	}

    void ProcessNode(aiNode* node, const aiScene* scene, Model& model, const std::string& file_directory)
    {
        // process all the node's meshes (if any)
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            model.AddMesh(ProcessMesh(mesh, scene, file_directory));
        }
        // then do the same for each of its children
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            ProcessNode(node->mChildren[i], scene, model, file_directory);
        }
    }

    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene, const std::string& file_directory)
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture2DMeshHandle> textures;

        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex{};
            // process vertex positions
            Vec3 vec;
            vec.x = mesh->mVertices[i].x;
            vec.y = mesh->mVertices[i].y;
            vec.z = mesh->mVertices[i].z;
            vertex.position = vec;

            // normals
            if (mesh->HasNormals()) {
                vec.x = mesh->mNormals[i].x;
                vec.y = mesh->mNormals[i].y;
                vec.z = mesh->mNormals[i].z;
                vertex.vertex_normal = vec;
            }

            // texture coordinates
            if (mesh->mTextureCoords[0]) {
                Vec2 tex_coord;
                tex_coord.x = mesh->mTextureCoords[0][i].x;
                tex_coord.y = mesh->mTextureCoords[0][i].y;
                vertex.texture_coordinate = tex_coord;
            }

			// tangent and bitangent
			if (mesh->HasTangentsAndBitangents()){
				Vec3 tangent, bitangent;
				tangent.x = mesh->mTangents[i].x;
				tangent.y = mesh->mTangents[i].y;
				tangent.z = mesh->mTangents[i].z;

				bitangent.x = mesh->mBitangents[i].x;
				bitangent.y = mesh->mBitangents[i].y;
				bitangent.z = mesh->mBitangents[i].z;

				// needs more test
				if (glm::dot(glm::cross(vertex.vertex_normal, tangent), bitangent) < 0.0f){
					tangent = tangent * -1.0f;
				}

				vertex.tangent = tangent;
			}

            vertices.push_back(vertex);
        }
        
        // indices
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        // process material
        if (mesh->mMaterialIndex >= 0)
        {
            aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
            LoadTexture(mat, aiTextureType_DIFFUSE, Texture2DUsage::DIFFUSE, file_directory, textures);
            LoadTexture(mat, aiTextureType_METALNESS, Texture2DUsage::SPECULAR, file_directory, textures);
            LoadTexture(mat, aiTextureType_NORMALS, Texture2DUsage::NORMAL, file_directory, textures);
        }

        return Mesh{ std::move(vertices), std::move(indices), std::move(textures) };
    }

    void LoadTexture(aiMaterial* mat, aiTextureType texture_type, Texture2DUsage engine_usage,
                     const std::string& file_directory, std::vector<Texture2DMeshHandle>& textures){
        for (unsigned int i = 0; i < mat->GetTextureCount(texture_type); i++)
        {
            aiString str;
            mat->GetTexture(texture_type, i, &str);
            std::string filename = file_directory + '/' + str.C_Str();
            Texture2DMeshHandle texture2DMeshHandle;
            texture2DMeshHandle.file_path = filename;
            texture2DMeshHandle.usage = engine_usage;
            textures.push_back(texture2DMeshHandle);
        }
    }
}