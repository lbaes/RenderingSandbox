#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "ModelLoader.h"
#include <iostream>

namespace Eng {

    void processNode(aiNode* node, const aiScene* scene, Model& model, ResourceCache<Texture2DHandle>* tex_cache);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene, ResourceCache<Texture2DHandle>* tex_cache);
    
    ModelLoader::ModelLoader(ResourceCache<Texture2D>* tex_cache) : texture_cache{tex_cache}
    {
    }

    ResourceHandle<Model> ModelLoader::LoadModel(const std::string& path) const
	{
        Assimp::Importer import;
        const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
            return;
        }
        //directory = path.substr(0, path.find_last_of('/'));

	    Model* model = new Model();
        ResourceHandle<Model> model_handle;
        model_handle.resource = std::move(std::shared_ptr<Model>(model));
        processNode(scene->mRootNode, scene, *model, texture_cache);
		return ;
	}

    void processNode(aiNode* node, const aiScene* scene, Model& model, ResourceCache<Texture2D>* tex_cache)
    {
        // process all the node's meshes (if any)
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            model.AddMesh(processMesh(mesh, scene, tex_cache));
        }
        // then do the same for each of its children
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene, model, tex_cache);
        }
    }

    Mesh processMesh(aiMesh* mesh, const aiScene* scene, ResourceCache<Texture2D>* tex_cache)
    {
        VertexBuffer vertices;
        IndexBuffer indices;
        TextureBuffer textures;
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
            vec.x = mesh->mNormals[i].x;
            vec.y = mesh->mNormals[i].y;
            vec.z = mesh->mNormals[i].z;
            vertex.vertex_normal = vec;

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
            
            vertices.AddVertex(vertex);
        }
        
        // indices
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.AddIndex(face.mIndices[j]);
        }

        // process material
        if (mesh->mMaterialIndex >= 0)
        {
            aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
            for (unsigned int i = 0; i < mat->GetTextureCount(aiTextureType_DIFFUSE); i++)
            {
                aiString str;
                mat->GetTexture(aiTextureType_DIFFUSE, i, &str);
                // TODO Rethink this -> weird and wasteful >:(
                auto texture = tex_cache->Fetch(str.C_Str());
                textures.AddTexture(*texture.resource);
            }
        }

        return Mesh{ vertices, indices, textures };
    }
}