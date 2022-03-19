#pragma once

namespace Eng {

	class VertexBuffer;
	class IndexBuffer;
	class Texture2D;
	class Shader;
	class Mesh;
	class Model;
	class RenderDevice {
	public:
	public:
		void InitRenderDevice();
		BufferGPUHandle CreateBuffers(const VertexBuffer& vertexBuffer, const IndexBuffer& indexBuffer) const;
		Texture2DGPUHandle CreateTexture2D(const Texture2D& tex) const;
		ShaderGPUHandle CreateShaderProgram(const Shader& vertexShader, const Shader& fragmentShader) const;
		MeshGPUHandle CreateMesh(const Mesh& mesh) const;
		ModelGPUHandle CreateModel(const Model& model) const;
	private:
		bool initialized = false;
	};
}