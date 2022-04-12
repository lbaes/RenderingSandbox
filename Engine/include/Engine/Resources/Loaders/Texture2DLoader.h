#pragma once
#include <string>
#include <stdexcept>
#include <vector>
#include <unordered_map>

namespace Eng {
	class Texture2D;
	class Texture2DLoader {
	public:
		static void Load(const std::string& filePath, Texture2D& outTexture);
		static void Unload(Texture2D& texture);
	};

	class ImageLoaderException : public std::runtime_error{
	public:
		ImageLoaderException(const std::string& message) : std::runtime_error(message) {

		}
	};
}