#include "Texture2D.h"
#include <Engine/Resources/ImageLoader.h>
#include <Engine/Core/Logger.h>

namespace Eng {

	int Texture2D::GetWidth() const
	{
		return img.width;
	}

	int Texture2D::GetHeight() const
	{
		return img.height;
	}

	unsigned char* Texture2D::GetData() const
	{
		return img.data;
	}

	Texture2D Texture2D::LoadFromDisk(const std::string& path)
	{
		Texture2D t;
		t.img = ImageLoader::Load(path, ImageLoader::Components::RGBA);
		return t;
	}
}
