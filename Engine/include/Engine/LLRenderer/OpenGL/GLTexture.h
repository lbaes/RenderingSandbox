#pragma once
#include <glad/glad.h>
#include "Engine/Resources/Texture2DMeshHandle.h"

namespace Eng::ogl {
    struct GLTexture {
        GLuint id;
        Texture2DUsage usage;
    };
}