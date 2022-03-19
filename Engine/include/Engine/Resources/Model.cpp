#include "Engine/Resources/Model.h"

namespace Eng {
	
	Model::Model(const std::vector<Mesh>& m) : meshes{ m } {

	}
	
	void Model::AddMesh(const Mesh& mesh) {
		meshes.push_back(mesh);
	}

	const std::vector<Mesh> Model::GetMeshes() const
	{
		return meshes;
	}

	Model Model::LoadFromDisk(const std::string& path) {
		return Model{};
	}
}
