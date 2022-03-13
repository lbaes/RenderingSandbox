#pragma once

namespace Eng {

	class VertexBuffer;
	class Texture2D;
	class RenderDevice {
	public:
	public:
		void InitRenderDevice();
		int CreateTexture2D(const Texture2D& tex) const;
		int CreateVertexBuffer(const VertexBuffer& vertexBuffer) const;
	private:
		bool initialized = false;
	};
}