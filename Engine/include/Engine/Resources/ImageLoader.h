#pragma once
#include <string>
#include <stdexcept>
#include "Image.h"

namespace Eng {
	class ImageLoader {
	public:
		enum class Components {
			GREYSCALE = 1,
			GREYSCALE_ALPHA = 2,
			RGB = 3,
			RGBA = 4,
		};
		static Image Load(const std::string& filePath, Components components);
		static void Unload(const Image& image) noexcept;
	};

	class ImageLoaderException : public std::runtime_error{
	public:
		ImageLoaderException(const std::string& message) : std::runtime_error(message) {

		}
	};
}