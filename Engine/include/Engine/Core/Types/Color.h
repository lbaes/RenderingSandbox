#pragma once
#include <Engine/Core/Types/Vec.h>

namespace Eng {

	class Color3 {
	public:
		Color3(float r = 0, float g = 0, float b = 0) {
			rgb_values[0] = r;
			rgb_values[1] = g;
			rgb_values[2] = b;
		}
		constexpr float red() const {
			return rgb_values.x;
		}
		constexpr float green() const {
			return rgb_values.y;
		};
		constexpr float blue() const {
			return rgb_values.z;
		};
	private:
		Vec3 rgb_values{};
	};

	class Color4 {
	public:
		Color4(float r = 0, float g = 0, float b = 0, float a = 1) {
			rgba_values[0] = r;
			rgba_values[1] = g;
			rgba_values[2] = b;
			rgba_values[3] = a;
		}

		constexpr float red() const {
			return rgba_values.x;
		}
		constexpr float green() const {
			return rgba_values.y;
		};
		constexpr float blue() const {
			return rgba_values.z;
		};
		constexpr float alpha() const {
			return rgba_values.w;
		};

	private:
		Vec4 rgba_values{};
	};
}