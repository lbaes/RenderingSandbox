#include "Texture2D.h"
#include <Engine/Resources/ImageLoader.h>
#include <Engine/Core/Logger.h>
#include <cstdlib>

namespace Eng {

	Texture2D::Texture2D() : usage{ Texture2DUsage::DIFFUSE } {}

	Texture2D::Texture2D(const Texture2D& other) : usage{ other.usage }
	{
		const size_t img_data_size = 4ll * other.img.height * other.img.width;
		img = other.img;
		file_path = other.file_path;
		img.data = (unsigned char*)new unsigned char[img_data_size]();
		if (other.img.data != nullptr) {
			memcpy(img.data, other.img.data, img_data_size);
		}
	}

	Texture2D::Texture2D(Texture2D&& other) noexcept
		: usage{ other.usage },
		img{ std::move(other.img) },
		file_path(std::move(other.file_path))
	{
		other.img.data = nullptr;
	}

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

	/// <summary>
	/// Creates a Texture2D object.
	/// </summary>
	/// <param name="path">Image file that will be loaded as texture</param>
	/// <returns>Returns a pointer to a heap allocated Texture2D*. Caller is responsible for freeing it.</returns>
	Texture2D* Texture2D::LoadFromDisk(const std::string& path)
	{
		Texture2D* t = nullptr;
		try {
			t = new Texture2D();
			t->img = ImageLoader::Load(path, ImageLoader::Components::RGBA);
			t->file_path = path;
			return t;
		}
		catch (const std::exception& e) {
			delete t;
			throw;
		}
	}

	Texture2D::~Texture2D()
	{
		ImageLoader::Unload(img);
		img.data = nullptr;
	}
}
