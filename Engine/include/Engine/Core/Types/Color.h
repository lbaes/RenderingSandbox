#pragma once
#include <Engine/Core/Types/Vec.h>

namespace Eng {

	class Color3 {
	public:
		Color3(unsigned int r = 0, unsigned int g = 0, unsigned int b = 0) {
			rgb_values[0] = r;
			rgb_values[1] = g;
			rgb_values[2] = b;
		}
		constexpr unsigned int red() const {
			return rgb_values.x;
		}
		constexpr unsigned int green() const {
			return rgb_values.y;
		};
		constexpr unsigned int blue() const {
			return rgb_values.z;
		};
	private:
		Vec3ui rgb_values{};
	};

	class Color4 {
	public:
		Color4(unsigned int r = 0, unsigned int g = 0, unsigned int b = 0, unsigned int a = 1) {
			rgba_values[0] = r;
			rgba_values[1] = g;
			rgba_values[2] = b;
			rgba_values[3] = a;
		}

		constexpr unsigned int red() const {
			return rgba_values.x;
		}
		constexpr unsigned int green() const {
			return rgba_values.y;
		};
		constexpr unsigned int blue() const {
			return rgba_values.z;
		};
		constexpr unsigned int alpha() const {
			return rgba_values.w;
		};

	private:
		Vec4ui rgba_values{};
	};
}