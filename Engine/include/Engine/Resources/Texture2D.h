#pragma once
#include <Engine/Resources/Image.h>
#include <string>

namespace Eng {

	enum class Texture2DUsage
	{
		DIFFUSE,
		SPECULAR,
		NORMAL,
	};

	class Texture2D {
	public:
		Texture2D();
		Texture2D(const Texture2D& other);
		Texture2D(Texture2D&& other) noexcept;
		~Texture2D();
		int GetWidth() const;
		int GetHeight() const;
		const Texture2DUsage usage;
		unsigned char* GetData() const;
		static Texture2D* LoadFromDisk(const std::string& path);
	private:
		Image img;
	};
}