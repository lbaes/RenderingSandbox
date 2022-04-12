#pragma once
#include <Engine/Core/Types/Vec.h>

namespace Eng {
	struct Vertex {
        Vec3 position{};
        Vec3 vertex_normal{};
        Vec2 texture_coordinate{};
		Vec3 tangent{};
	};
}