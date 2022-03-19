#pragma once
#include <cstdint>
#include <vector>

namespace Eng {

	struct Texture2DGPUHandle 
	{
		unsigned int ID = 0;
		//uint32_t file_key = 0;
	};

	struct ShaderGPUHandle
	{
		unsigned int ID = 0;
	};

	struct MeshGPUHandle
	{
		unsigned int VAO = 0;
		unsigned int VBO = 0;
		unsigned int EBO = 0;
		std::vector<Texture2DGPUHandle> textures;
	};

	struct BufferGPUHandle
	{
		unsigned int VAO = 0;
		unsigned int VBO = 0;
		unsigned int EBO = 0;
	};

	struct ModelGPUHandle
	{
		std::vector<Mesh> meshes;
	};
}