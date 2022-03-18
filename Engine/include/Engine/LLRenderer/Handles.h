#pragma once

namespace Eng {

	struct Texture2DHandle 
	{
		unsigned int ID = 0;
	};

	struct ShaderHandle
	{
		unsigned int ID = 0;
	};

	struct MeshHandle
	{
		unsigned int VAO = 0;
		unsigned int VBO = 0;
		unsigned int EBO = 0;
	};

	struct BufferHandle
	{
		unsigned int VAO = 0;
		unsigned int VBO = 0;
		unsigned int EBO = 0;
	};

	struct ModelHandle
	{
		unsigned int VAO = 0;
	};
}