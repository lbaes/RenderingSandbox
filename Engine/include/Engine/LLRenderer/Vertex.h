#pragma once
#include <Engine/Core/Types/Vec.h>
#include <Engine/Core/Types/Color.h>

namespace Eng {
	struct Vertex {
		Vertex() = default;
		Vertex(Vec3 pos, Color4 diffuse, Color4 specular, Vec3 normal, Vec2 tex_coord) :
			position{ pos }, 
			diffuse_color{ diffuse }, 
			specular_color{ specular }, 
			vertex_normal{ normal }, 
			texture_coordinate{ tex_coord }{};
		Vec3 position;
		Color4 diffuse_color;
		Color4 specular_color;
		Vec3 vertex_normal;
		Vec2 texture_coordinate;
	};
}