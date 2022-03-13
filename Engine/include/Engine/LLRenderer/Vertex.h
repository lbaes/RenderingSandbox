#pragma once
#include <Engine/Core/Types/Vec.h>
#include <Engine/Core/Types/Color.h>

namespace Eng {
	struct Vertex {
		Vec3 position;
		Color4 diffuse_color;
		Color4 specular_color;
		Vec3 vertex_normal;
		Vec2 texture_coordinate;
	};
}