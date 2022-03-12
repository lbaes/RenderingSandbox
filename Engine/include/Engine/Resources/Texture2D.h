#pragma once
#include <Engine/Resources/ResourceCache.h>
#include <Engine/Resources/Image.h>
#include <string>

namespace Eng {
	class Texture2D {
	public:
		Texture2D() = default;
		int GetWidth() const;
		int GetHeight() const;
		unsigned char* GetData() const;
		static Texture2D LoadFromDisk(const std::string& path);
	private:
		Image img;
	};
}