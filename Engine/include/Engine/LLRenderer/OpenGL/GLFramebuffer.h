#pragma once

namespace Eng::ogl
{
	enum GLFrameBufferAttachments {
		COLOR 	= 1u << 0,
		DEPTH 	= 1u << 1,
		STENCIL = 1u << 2,
	};

	enum GLFrameBufferOptions {
		SRGB_COLOR = 1u << 0
	};

	struct GLFramebuffer
	{
		unsigned int ID = 0;
		unsigned int COLOR_ID = 0;
		unsigned int DEPTH_ID = 0;
		unsigned int STENCIL_ID = 0;
		bool isSRGB = false;
		void Bind() const;
		void Unbind() const;
		unsigned int attachments = GLFrameBufferAttachments::COLOR;
		int width = 0, height = 0;
	};
}
