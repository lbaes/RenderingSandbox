#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <string>
#include "ModelLoader.h"
#include "Engine/Resources/Mesh.h"
#include "Engine/Resources/Model.h"

namespace Eng {

    void ProcessNode(aiNode* node, const aiScene* scene, Model& model, const std::string& file_directory);
    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene, const std::string& file_directory);

    void ModelLoader::LoadModel(const std::string& path, Model& model )
	{
        Assimp::Importer import;
        const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
            throw std::runtime_error("RESOURCE LOAD ERROR");
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

        // Normals
        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
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
            else {
                vertex.texture_coordinate = Vec2{ 0.0, 0.0 };
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
            for (unsigned int i = 0; i < mat->GetTextureCount(aiTextureType_DIFFUSE); i++)
            {
                aiString str;
                mat->GetTexture(aiTextureType_DIFFUSE, i, &str);
                std::string filename = file_directory + '/' + str.C_Str();
                Texture2DMeshHandle texture2DMeshHandle;
                texture2DMeshHandle.file_path = filename;
                texture2DMeshHandle.usage = Texture2DUsage::DIFFUSE;
                textures.push_back(texture2DMeshHandle);
            }
        }

        return Mesh{ std::move(vertices), std::move(indices), std::move(textures) };
    }
}