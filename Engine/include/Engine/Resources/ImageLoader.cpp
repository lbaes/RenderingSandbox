#include "ImageLoader.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <sstream>

/// <summary>
/// Loads an image file from filePath; Supports JPEG, PNG, BMP, TGA
/// </summary>
/// <param name="filePath">File location on disk </param>
/// <param name="components">Desired components per pixel</param>
/// <returns></returns>
Eng::Image Eng::ImageLoader::Load(const std::string& filePath, Components components)
{
	Image img{};
	switch (components)
	{
	case Eng::ImageLoader::Components::GREYSCALE:
		img.data = stbi_load(filePath.c_str(), &img.width, &img.height, &img.num_channels_in_file, 1);
		break;
	case Eng::ImageLoader::Components::GREYSCALE_ALPHA:
		img.data = stbi_load(filePath.c_str(), &img.width, &img.height, &img.num_channels_in_file, 2);
		break;
	case Eng::ImageLoader::Components::RGB:
		img.data = stbi_load(filePath.c_str(), &img.width, &img.height, &img.num_channels_in_file, 3);
		break;
	case Eng::ImageLoader::Components::RGBA:
		img.data = stbi_load(filePath.c_str(), &img.width, &img.height, &img.num_channels_in_file, 4);
		break;
	default:
		img.data = nullptr;
		break;
	}
	if (img.data == nullptr) {
		std::stringstream error_msg;
		error_msg << "Failed to load requested image: \"" << filePath << "\". Reason: "<< stbi_failure_reason() << "\n";
		throw ImageLoaderException(error_msg.str());
	}
	return img;
}

void Eng::ImageLoader::Unload(const Image& image) noexcept
{
	stbi_image_free(image.data);
}
