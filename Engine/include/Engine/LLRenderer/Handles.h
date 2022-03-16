#pragma once

namespace Eng {
	
	enum class Texture2DUsage 
	{
		DIFFUSE,
		SPECULAR,
		NORMAL,
	};

	struct Texture2DHandle 
	{
		unsigned int ID = 0;
		Texture2DUsage usage = Texture2DUsage::DIFFUSE;
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
}