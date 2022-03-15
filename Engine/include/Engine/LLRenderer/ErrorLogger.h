#pragma once
#include <Engine/Core/Logger.h>
#include <glad/glad.h>

namespace Eng {
	class ErrorLogger {
	public:
		static void gl_error_logger(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
			auto logger = Logger::GetLogger();
			switch (severity)
			{
			case GL_DEBUG_SEVERITY_HIGH:
				logger->LogError("GL DEBUG: {}", std::string(message));
				break;
			case GL_DEBUG_SEVERITY_MEDIUM:
				logger->LogError("GL DEBUG: {}", std::string(message));
				break;
			case GL_DEBUG_SEVERITY_LOW:
				logger->LogInfo("GL DEBUG: {}", std::string(message));
				break;
			case GL_DEBUG_SEVERITY_NOTIFICATION:
				logger->LogInfo("GL DEBUG: {}", std::string(message));
				break;
			default:
				break;
			}
		};
	};
}
