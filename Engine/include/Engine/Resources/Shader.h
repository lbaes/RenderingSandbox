#pragma once
#include <Engine/Resources/ResourceCache.h>
#include <string>

namespace Eng {

	enum class ShaderType {
		VERTEX,
		FRAGMENT,
		GEOMETRY,
		TESSELATION,
		INVALID,
	};

	class Shader {
	public:
		static Shader LoadFromDisk(const std::string& path);
		const std::string& GetShaderSource() const;
		ShaderType GetShaderType() const;
	private:
		Shader() = default;
		std::string shader_source = "";
		ShaderType shader_type = ShaderType::INVALID;
	};
}