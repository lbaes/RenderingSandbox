#include "Texture2D.h"
#include "Loaders/Texture2DLoader.h"

namespace Eng {

	Texture2D::Texture2D(Texture2D&& other) noexcept :
	_width{other._width}, _height{other._height}, _nr_channels{other._nr_channels}, _data(other._data), _file_path(std::move(other._file_path))
	{
		other._data = nullptr;
	}

	Texture2D::Texture2D(const Texture2D& other) :
	_width{other._width}, _height{other._height}, _nr_channels{other._nr_channels}, _file_path(other._file_path)
	{
		const size_t size = other._width * other._height * other._nr_channels * sizeof(unsigned char*);
		_data = new unsigned char[size];
		for (size_t i = 0; i < size; ++i)
		{
			_data[i] = other._data[i];
		}
	}

    Texture2D::~Texture2D() {
        Texture2DLoader::Unload(*this);
        _data = nullptr;
    }

	void Texture2D::LoadFromFile(const std::string& filePath)
	{
		Texture2DLoader::Load(filePath, *this);
	}

	int Texture2D::GetWidth() const
	{
		return _width;
	}

	int Texture2D::GetHeight() const
	{
		return _height;
	}

	int Texture2D::GetNumberOfChannels() const
	{
		return _nr_channels;
	}

	const std::string& Texture2D::GetPath() const
	{
		return _file_path;
	}

	unsigned char* Texture2D::GetData() const
	{
		return _data;
	}
}
