#include "Shader.h"
#include <stdexcept>
#include <sstream>
#include <Engine/Utility/ReadFile.h>

namespace Eng {
	ShaderType GetShaderTypeFromFileName(const std::string& file_path);
	void Shader::LoadFromDisk(const std::string& path)
	{
		source = Util::read_file(path);
		shader_type = GetShaderTypeFromFileName(path);
	}

	std::string Shader::GetShaderSourceCodeComplete() const
	{
		if (source.empty())
			throw std::runtime_error("SHADER WAS NOT INITIALIZED OR IS EMPTY");
		return defines + source;
	}

	ShaderType Shader::GetShaderType() const
	{
		if (ShaderType::INVALID == shader_type)
			throw std::runtime_error("SHADER WAS NOT INITIALIZED");
		return shader_type;
	}

	void Shader::ConfigureEffects(unsigned int effects)
	{
		_effects = effects;
		defines = "#version 450 core\n";
		if (effects & ShaderEffects::TEXTURES){
			defines += "#define USE_DIFFUSE_TEXTURE\n";
		}
		if (effects & ShaderEffects::NORMAL_MAP){
			defines += "#define USE_NORMAL_MAP\n";
		}
		if (effects & ShaderEffects::SPECULAR_MAP){
			defines += "#define USE_SPECULAR_MAP\n";
		}
		if (effects & ShaderEffects::AMBIENT_LIGHT){
			defines += "#define USE_AMBIENT_LIGHT\n";
		}
	}

	const std::string& Shader::GetShaderDefines() const
	{
		return defines;
	}

	const std::string& Shader::GetShaderSourceCode() const
	{
		return source;
	}

	unsigned int Shader::GetShaderEffects() const
	{
		return _effects;
	}

	ShaderType GetShaderTypeFromFileName(const std::string& file_path) {
        auto firstCharIndex = file_path.find_last_of('/') + 1;
        if (firstCharIndex >= file_path.size())
            goto error_label;
		switch (file_path[firstCharIndex])
		{
		case 'f':
			return ShaderType::FRAGMENT;
		case 'v':
			return ShaderType::VERTEX;
		case 't':
			return ShaderType::TESSELATION;
		case 'g':
			return ShaderType::GEOMETRY;
		default:
            error_label:
			std::stringstream error_msg;
			error_msg << "COULD NOT DETERMINE SHADER TYPE FROM FILENAME: " << file_path;
			throw std::runtime_error(error_msg.str());
		}
	}
}