#include "RenderDevice.h"
#include "Engine/Resources/Texture2D.h"
#include "Engine/LLRenderer/Buffers.h"
#include "Engine/Core/Types/HashedString.h"
#include "glad/glad.h"


namespace Eng {

	constexpr GLenum GetGLUsage(VERTEX_BUFFER_USAGE usage);

	void RenderDevice::InitRenderDevice()
	{
		initialized = true;
	}

	/// <summary>
	/// Creates a 2D Texture resource on the GPU
	/// </summary>
	/// <param name="tex"></param>
	/// <returns>ID used to refer to the texture</returns>
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

		return texture_id;
	}

	int RenderDevice::CreateBuffers(const VertexBuffer& vertexBuffer, const IndexBuffer& indexBuffer) const
	{
		unsigned int vbo_id, vao_id, ebo_id;
		glGenBuffers(1, &vbo_id);
		glGenBuffers(1, &ebo_id);
		glGenVertexArrays(1, &vao_id);

		// Bind VAO
		glBindVertexArray(vao_id);

		// Copy vertex data to VBO
		glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
		glBufferData(GL_ARRAY_BUFFER, vertexBuffer.GetSizeBytes(), vertexBuffer.GetData(), GetGLUsage(vertexBuffer.GetUsage()));

		// Copy index data to EBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBuffer.GetSizeBytes(), indexBuffer.GetData(), GL_STATIC_DRAW);

		// Unbind VAO
		glBindVertexArray(0);

		// Unbind buffers
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		using namespace Eng::literal;
		auto hstring = "test"_hash;

		return vao_id;
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