#pragma once

#include <glm/gtc/type_ptr.hpp>
#include "Engine/Core/Types/Mat.h"
#include "Engine/Core/Types/Vec.h"
#include "glad/glad.h"

namespace Eng::ogl
{
	struct GLShader
	{
		GLuint id;
		unsigned int effects;

		void use() const
		{
			glUseProgram(id);
		}

		void uniform_set(const std::string& uniformName, bool value) const
		{
			glUniform1i(glGetUniformLocation(id, uniformName.c_str()), value);
		}

		void uniform_set(const std::string& uniformName, int value) const
		{
			glUniform1i(glGetUniformLocation(id, uniformName.c_str()), value);
		}

		void uniform_set(const std::string& uniformName, float value) const
		{
			glUniform1f(glGetUniformLocation(id, uniformName.c_str()), value);
		}

		void uniform_set(const std::string& uniformName, Vec2 value) const
		{
			glUniform2f(glGetUniformLocation(id, uniformName.c_str()), value.x, value.y);
		}

		void uniform_set(const std::string& uniformName, Vec3 value) const
		{
			glUniform3f(glGetUniformLocation(id, uniformName.c_str()), value.x, value.y, value.z);
		}

		void uniform_set(const std::string& uniformName, Vec4 value) const
		{
			glUniform4f(glGetUniformLocation(id, uniformName.c_str()), value.x, value.y, value.z, value.w);
		}

		void uniform_set(const std::string& uniformName, Mat3 value) const
		{
			glUniformMatrix3fv(glGetUniformLocation(id, uniformName.c_str()), 1, false, glm::value_ptr(value));
		}

		void uniform_set(const std::string& uniformName, Mat4 value) const
		{
			glUniformMatrix4fv(glGetUniformLocation(id, uniformName.c_str()), 1, false, glm::value_ptr(value));
		}

		template<typename Value>
		void
		uniform_set_structArray_member(const std::string& uniformName, int i, const std::string& member, Value value)
		{
			std::string indexed_uniform = uniformName + "[" + std::to_string(i) + "]." + member;
			uniform_set(indexed_uniform, value);
		}

		template<typename Value>
		void
		uniform_set_array(const std::string& uniformName, int i, Value value)
		{
			std::string indexed_uniform = uniformName + "[" + std::to_string(i) + "]";
			uniform_set(indexed_uniform, value);
		}
	};
}