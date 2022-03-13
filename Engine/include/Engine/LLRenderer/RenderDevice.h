#pragma once

namespace Eng {

	class VertexBuffer;
	class IndexBuffer;
	class Texture2D;
	class RenderDevice {
	public:
	public:
		void InitRenderDevice();
		int CreateTexture2D(const Texture2D& tex) const;
		int CreateBuffers(const VertexBuffer& vertexBuffer, const IndexBuffer& indexBuffer) const;
	private:
		bool initialized = false;
	};
}