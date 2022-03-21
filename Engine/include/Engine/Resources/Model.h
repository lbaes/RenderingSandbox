#pragma once
#include "Mesh.h"
#include <vector>

namespace Eng {
	class Model {
	public:
		Model() = default;
        Model(Model&& other) noexcept = default;
        Model(const Model& other) = default;
        void LoadFromFile(const std::string& filepath);
        void LoadFromMeshes(std::vector<Mesh> meshes_);
		const std::vector<Mesh>& GetMeshes() const;
        void AddMesh(const Mesh& mesh);
    private:
		std::vector<Mesh> meshes{};
        std::string file_path;
    };
}