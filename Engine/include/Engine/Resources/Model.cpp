#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Engine/Resources/Model.h"

namespace Eng {
	
	Model::Model(const std::vector<Mesh>& m) : meshes{ m } {

	}
	
	void Model::AddMesh(const Mesh& mesh) {
		meshes.push_back(mesh);
	}

	Model Model::LoadFromDisk(const std::string& path) {


		return Model{};
	}
}
