#include "RenderDevice.h"
#include "Engine/Resources/Texture2D.h"
#include "Engine/LLRenderer/Buffers.h"
#include "glad/glad.h"


namespace Eng {

	constexpr GLenum GetGLUsage(VERTEX_BUFFER_USAGE usage);

	void RenderDevice::InitRenderDevice()
	{
		initialized = true;
	}

	int RenderDevice::CreateTexture2D(const Texture2D& tex) const
	{
		unsigned int texture_id;
		glGenTextures(1, &texture_id);
		glBindTexture(GL_TEXTURE_2D, texture_id);

		// TODO: make parameters configurable
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Copy bytes to opengl texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex.GetWidth(), tex.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.GetData());

		// Mipmapping
		glGenerateMipmap(GL_TEXTURE_2D);

		return 0;
	}

	int RenderDevice::CreateVertexBuffer(const VertexBuffer& vertexBuffer) const
	{
		unsigned int vbo_id, vao_id, ebo_id;
		glGenBuffers(1, &vbo_id);
		glGenBuffers(1, &ebo_id);
		glGenVertexArrays(1, &vao_id);

		// Bind VAO
		glBindVertexArray(vao_id);

		// Copy data
		glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
		glBufferData(GL_ARRAY_BUFFER, vertexBuffer.GetSizeBytes(), vertexBuffer.GetData(), GetGLUsage(vertexBuffer.GetUsage()));


		return 0;
	}

	constexpr GLenum GetGLUsage(VERTEX_BUFFER_USAGE usage) {
		switch (usage)
		{
		case Eng::VERTEX_BUFFER_USAGE::DYNAMIC:
			return GL_DYNAMIC_DRAW;
			break;
		case Eng::VERTEX_BUFFER_USAGE::STATIC:
			return GL_STATIC_DRAW;
			break;
		default:
			return GL_STATIC_DRAW;
			break;
		}
	}
}