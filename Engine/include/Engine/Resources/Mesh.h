#pragma once
#include "Engine/LLRenderer/Buffers.h"

namespace Eng {
	class Mesh {
	public:
		VertexBuffer vertices;
		IndexBuffer indices;
		TextureHashes textures;

		Mesh(const VertexBuffer& v, const IndexBuffer& i, const TextureHashes& tb)
		 : vertices(v), indices(i), textures(tb){}
	};
}