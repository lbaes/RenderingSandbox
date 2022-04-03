#pragma once
#include <string>

namespace Eng {

	enum class ShaderType {
		VERTEX,
		FRAGMENT,
		GEOMETRY,
		TESSELATION,
		INVALID,
	};

	enum ShaderEffects {
		TEXTURES 		= 1u << 0,
		SPECULAR_MAP 	= 1u << 1,
		NORMAL_MAP		= 1u << 2,
		AMBIENT_LIGHT	= 1u << 3
	};

	class Shader {
	public:
		Shader() = default;
		void ConfigureEffects(unsigned int effects);
		void LoadFromDisk(const std::string& path);
		std::string GetShaderSourceCodeComplete() const;
		const std::string& GetShaderSourceCode() const;
		const std::string& GetShaderDefines() const;
		ShaderType GetShaderType() const;
	private:
		std::string source;
		std::string defines = "#version 450 core\n";
		ShaderType shader_type = ShaderType::INVALID;
	};
}