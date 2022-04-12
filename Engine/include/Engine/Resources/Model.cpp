#include "Engine/Resources/Model.h"
#include "Engine/Resources/Loaders/ModelLoader.h"

#include <utility>

namespace Eng {

    void Model::LoadFromFile(const std::string &filepath) {
        ModelLoader loader;
        loader.LoadModel(filepath, *this);
    }

    void Model::LoadFromMeshes(std::vector<Mesh> meshes_) {
        file_path = "";
        this->meshes = std::move(meshes_);
    }

    const std::vector<Mesh> &Model::GetMeshes() const {
        return meshes;
    }

    void Model::AddMesh(const Mesh& mesh) {
        meshes.push_back(mesh);
    }
}
