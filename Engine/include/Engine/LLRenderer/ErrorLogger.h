#pragma once
#include <Engine/Core/Logger.h>
#include <glad/glad.h>

namespace Eng {
	class ErrorLogger {
	public:
		static void gl_error_logger([[maybe_unused]]GLenum source, [[maybe_unused]]GLenum type, [[maybe_unused]]GLuint id, [[maybe_unused]]GLenum severity, [[maybe_unused]]GLsizei length, const GLchar* message, [[maybe_unused]]const void* userParam) {
			auto logger = Logger::GetLogger();
			switch (severity)
			{
			case GL_DEBUG_SEVERITY_HIGH:
            case GL_DEBUG_SEVERITY_MEDIUM:
				logger->LogError("GL DEBUG: {}", std::string(message));
				break;
			case GL_DEBUG_SEVERITY_LOW:
            case GL_DEBUG_SEVERITY_NOTIFICATION:
                logger->LogInfo("GL DEBUG: {}", std::string(message));
				break;
			default:
				break;
			}
		};
	};
}
