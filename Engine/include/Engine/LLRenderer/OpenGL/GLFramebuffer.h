#pragma once

namespace Eng::ogl {
    enum GLFrameBufferAttachments : unsigned int{
        COLOR = 1 << 0,
        DEPTH = 1 << 1,
        STENCIL = 1 << 2,
    };

    inline GLFrameBufferAttachments operator|(GLFrameBufferAttachments a, GLFrameBufferAttachments b) {
        return static_cast<GLFrameBufferAttachments>(static_cast<unsigned int>(a) | static_cast<unsigned int>(b));
    }

    inline GLFrameBufferAttachments operator&(GLFrameBufferAttachments a, GLFrameBufferAttachments b) {
        return static_cast<GLFrameBufferAttachments>(static_cast<unsigned int>(a) & static_cast<unsigned int>(b));
    }

    enum GLFrameBufferOptions {
        SRGB_COLOR = 1u << 0
    };

    struct GLFramebuffer {
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
