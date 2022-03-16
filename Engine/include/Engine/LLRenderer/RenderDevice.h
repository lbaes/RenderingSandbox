#pragma once
#include "Handles.h"

namespace Eng {

	class VertexBuffer;
	class IndexBuffer;
	class Texture2D;
	class Shader;
	class Mesh;
	class RenderDevice {
	public:
	public:
		void InitRenderDevice();
		BufferHandle CreateBuffers(const VertexBuffer& vertexBuffer, const IndexBuffer& indexBuffer) const;
		Texture2DHandle CreateTexture2D(const Texture2D& tex, Texture2DUsage usage) const;
		ShaderHandle CreateShaderProgram(const Shader& vertexShader, const Shader& fragmentShader) const;
		MeshHandle CreateMesh(const Mesh& mesh) const;
	private:
		bool initialized = false;
	};
}