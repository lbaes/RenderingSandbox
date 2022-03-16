#pragma once
#include "Buffers.h"

namespace Eng {
	class Mesh {
	public:
		VertexBuffer vertices;
		IndexBuffer indices;
		TextureBuffer textures;

		Mesh(const VertexBuffer& v, const IndexBuffer& i, const TextureBuffer& tb) 
		 : vertices(v), indices(i), textures(tb){}
	};
}