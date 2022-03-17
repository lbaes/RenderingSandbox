#pragma once
#include <Engine/Resources/ResourceCache.h>
#include <Engine/Resources/Image.h>
#include <string>

namespace Eng {
	class Texture2D {
	public:
		Texture2D() = default;
		~Texture2D();
		Texture2D(const Texture2D& other);
		Texture2D(Texture2D&& other) noexcept;
		int GetWidth() const;
		int GetHeight() const;
		unsigned char* GetData() const;
		static Texture2D* LoadFromDisk(const std::string& path);
	private:
		Image img;
	};
}