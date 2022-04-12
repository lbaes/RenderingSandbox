#pragma once

#include <glm/gtc/type_ptr.hpp>
#include <unordered_map>
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

		GLint get_uniform_location(const std::string& uniformName) const{
			GLint location;
			if (location_cache.find(uniformName) != location_cache.end())
				location = location_cache.at(uniformName);
			else{
				location = glGetUniformLocation(id, uniformName.c_str());
				location_cache[uniformName] = location;
			}
			return location;
		}

		void uniform_set(const std::string& uniformName, bool value) const
		{
			GLint location = get_uniform_location(uniformName);
			glUniform1i(location, value);
		}

		void uniform_set(const std::string& uniformName, int value) const
		{
			GLint location = get_uniform_location(uniformName);
			glUniform1i(location, value);
		}

		void uniform_set(const std::string& uniformName, float value) const
		{
			GLint location = get_uniform_location(uniformName);
			glUniform1f(location, value);
		}

		void uniform_set(const std::string& uniformName, Vec2 value) const
		{
			GLint location = get_uniform_location(uniformName);
			glUniform2f(location, value.x, value.y);
		}

		void uniform_set(const std::string& uniformName, Vec3 value) const
		{
			GLint location = get_uniform_location(uniformName);
			glUniform3f(location, value.x, value.y, value.z);
		}

		void uniform_set(const std::string& uniformName, Vec4 value) const
		{
			GLint location = get_uniform_location(uniformName);
			glUniform4f(location, value.x, value.y, value.z, value.w);
		}

		void uniform_set(const std::string& uniformName, Mat3 value) const
		{
			GLint location = get_uniform_location(uniformName);
			glUniformMatrix3fv(location, 1, false, glm::value_ptr(value));
		}

		void uniform_set(const std::string& uniformName, Mat4 value) const
		{
			GLint location = get_uniform_location(uniformName);
			glUniformMatrix4fv(location, 1, false, glm::value_ptr(value));
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

	private:
		mutable std::unordered_map<std::string, GLint> location_cache;
	};
}