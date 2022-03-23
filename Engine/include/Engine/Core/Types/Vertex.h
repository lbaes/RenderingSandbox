#pragma once
#include <Engine/Core/Types/Vec.h>
#include <Engine/Core/Types/Color.h>

namespace Eng {
	struct Vertex {
		Vertex() = default;
		Vertex(Vec3 pos, Vec3 normal, Vec2 tex_coord) :
			position{ pos }, 
            vertex_normal{ normal },
            texture_coordinate{ tex_coord }{};
        Vec3 position;
        Vec3 vertex_normal;
        Vec2 texture_coordinate;
        //Color4 diffuse_color;
        //Color4 specular_color;
	};
}