#pragma once

namespace Eng::ogl {
	struct GLQuad {
        GLQuad(float x, float y, float w, float h) : x{x}, y{y}, w{w}, h{h} {
        }
        float x, y, w, h;
	};
}
