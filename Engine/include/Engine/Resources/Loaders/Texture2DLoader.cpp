#include "Texture2DLoader.h"
#include "../Texture2D.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <sstream>

void Eng::Texture2DLoader::Load(const std::string& filePath, Eng::Texture2D& outTexture)
{
	outTexture._file_path = filePath;
    stbi_set_flip_vertically_on_load(false);
    auto data = stbi_load(filePath.c_str(), &outTexture._width, &outTexture._height, &outTexture._nr_channels, 0);
	if (data == nullptr) {
		std::stringstream error_msg;
		error_msg << "Failed to load requested image: \"" << filePath << "\". Reason: "<< stbi_failure_reason() << "\n";
		throw ImageLoaderException(error_msg.str());
	}
    outTexture._data = data;
}

void Eng::Texture2DLoader::Unload(Eng::Texture2D &texture) {
    if (texture._data == nullptr)
        stbi_image_free(texture._data);
}
