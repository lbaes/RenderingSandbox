#pragma once
#include "Mesh.h"
#include <vector>

namespace Eng {
	class Model {
	public:
		Model() = default;
		Model(const std::vector<Mesh>& mesh);
		void AddMesh(const Mesh& mesh);
		static Model LoadFromDisk(const std::string& file);
	private:
		std::vector<Mesh> meshes;
	};
}