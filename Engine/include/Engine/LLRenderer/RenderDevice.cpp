#include "RenderDevice.h"
#include "Engine/LLRenderer/Buffers.h"
#include "Engine/Resources/Texture2D.h"
#include "Engine/Resources/Mesh.h"
#include "Engine/Resources/Shader.h"
#include "ErrorLogger.h"
#include "glad/glad.h"

namespace Eng {

	constexpr GLenum GetGLUsage(VERTEX_BUFFER_USAGE usage);
	int CheckCompileErrors(unsigned int shader, const std::string& shaderType);
	int CheckLinkerErrors(unsigned int shader);

	void RenderDevice::InitRenderDevice()
	{
#ifdef ENGINE_DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(ErrorLogger::gl_error_logger, NULL);
#endif // ENGINE_DEBUG
		initialized = true;
	}

	/// <summary>
	/// Creates a 2D Texture resource on the GPU
	/// </summary>
	/// <param name="tex"></param>
	/// <returns>ID used to refer to the texture</returns>
	Texture2DHandle RenderDevice::CreateTexture2D(const Texture2D& tex, Texture2DUsage usage) const
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

		Texture2DHandle t;
		t.ID = texture_id;
		t.usage = usage;
		return t;
	}

	BufferHandle RenderDevice::CreateBuffers(const VertexBuffer& vertexBuffer, const IndexBuffer& indexBuffer) const
	{
		// Create Buffers
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

		BufferHandle bh;
		bh.VAO = vao_id;
		bh.VBO = vbo_id;
		bh.EBO = ebo_id;
		return bh;
	}

	ShaderHandle RenderDevice::CreateShaderProgram(const Shader& vertexShader, const Shader& fragmentShader) const
	{
		unsigned int vShaderID, fShaderID, programID;
		const auto fShaderCode = fragmentShader.GetShaderSource().c_str();
		const auto vShaderCode = vertexShader.GetShaderSource().c_str();

		// Vertex Shader
		vShaderID = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vShaderID, 1, &vShaderCode, NULL);
		glCompileShader(vShaderID);
		CheckCompileErrors(vShaderID, "VERTEX");

		// Fragment Shader
		fShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fShaderID, 1, &fShaderCode, NULL);
		glCompileShader(fShaderID);
		CheckCompileErrors(fShaderID, "FRAGMENT");

		// Shader Program
		programID = glCreateProgram();
		glAttachShader(programID, vShaderID);
		glAttachShader(programID, fShaderID);
		glLinkProgram(programID);
		CheckLinkerErrors(programID);

		// Delete shaders
		glDeleteShader(vShaderID);
		glDeleteShader(fShaderID);

		ShaderHandle sh;
		sh.ID = programID;

		return sh;
	}

	MeshHandle RenderDevice::CreateMesh(const Mesh& mesh) const
	{
		// Create Buffers
		BufferHandle bh = CreateBuffers(mesh.vertices, mesh.indices);

		// vertex positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		
		// vertex normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, vertex_normal));
		
		// vertex texture coordinates
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture_coordinate));

		// unbind
		glBindVertexArray(0);

		// This is weird
		MeshHandle m;
		m.VAO = bh.VAO;
		m.VBO = bh.VBO;
		m.EBO = bh.EBO;
		return m;
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

	// TODO: MOVE TO SHADER COMPILER CLASS
	int CheckCompileErrors(unsigned int shader, const std::string& shaderType) {
		int compilationSuccess;
		char infoLog[512];

		glGetShaderiv(shader, GL_COMPILE_STATUS, &compilationSuccess);
		if (!compilationSuccess)
		{
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::" << shaderType << "::COMPILATION_FAILED\n" << infoLog << std::endl;

		}

		return compilationSuccess;
	}

	// TODO: MOVE TO SHADER COMPILER CLASS
	int CheckLinkerErrors(unsigned int shader) {
		int linkSuccess;
		char infoLog[512];

		glGetProgramiv(shader, GL_LINK_STATUS, &linkSuccess);
		if (!linkSuccess)
		{
			glGetProgramInfoLog(shader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::" << "PROGRAM" << "::LINKING_FAILED\n" << infoLog << std::endl;

		}

		return linkSuccess;
	}
}