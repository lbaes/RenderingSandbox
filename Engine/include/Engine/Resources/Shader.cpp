#include "Shader.h"
#include <stdexcept>
#include <string>
#include <sstream>
#include <Engine/Utility/ReadFile.h>

namespace Eng {
	ShaderType GetShaderTypeFromFileName(const std::string& filename);

	Shader Shader::LoadFromDisk(const std::string& path)
	{
		Shader shader;
		shader.shader_source = Util::read_file(path);
		shader.shader_type = GetShaderTypeFromFileName(path);
		return shader;
	}

	const std::string& Shader::GetShaderSource() const
	{
		if ("" == shader_source)
			throw std::runtime_error("SHADER WAS NOT INITIALIZED OR IS EMPTY");
		return shader_source;
	}

	ShaderType Shader::GetShaderType() const
	{
		if (ShaderType::INVALID == shader_type)
			throw std::runtime_error("SHADER WAS NOT INITIALIZED");
		return shader_type;
	}

	ShaderType GetShaderTypeFromFileName(const std::string& filename) {
		auto firstChar = filename[0];
		switch (firstChar)
		{
		case 'f':
			return ShaderType::FRAGMENT;
			break;
		case 'v':
			return ShaderType::VERTEX;
			break;
		case 't':
			return ShaderType::TESSELATION;
			break;
		case 'g':
			return ShaderType::GEOMETRY;
			break;
		default:
			std::stringstream error_msg;
			error_msg << "COULD NOT DETERMINE SHADER TYPE FROM FILENAME: " << filename;
			throw std::runtime_error(error_msg.str());
			break;
		}
	}
}