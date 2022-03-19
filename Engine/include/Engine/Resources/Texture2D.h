#pragma once
#include <string>

namespace Eng {
	class Texture2D {
	public:
		Texture2D() = default;
		Texture2D(Texture2D&& other) noexcept ;
		Texture2D(const Texture2D& other);
		void LoadFromFile(const std::string& filePath);
		int GetWidth() const;
		int GetHeight() const;
		int GetNumberOfChannels() const;
		const std::string& GetPath() const;
		unsigned char* GetData() const;
		friend class Texture2DLoader;
	private:
		int _width = 0;
		int _height = 0;
		int _nr_channels = 0;
		unsigned char* _data = nullptr;
		std::string _file_path;
	};
}