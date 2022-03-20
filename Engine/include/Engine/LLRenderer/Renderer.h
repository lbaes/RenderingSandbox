#pragma once
#include "Engine/Core/Types/Transform.h"
#include "RenderDevice.h"

namespace Eng {
    class Renderer {
    public:
        void RenderModel(GPUModelHandle model, GPUShaderHandle shader, Transform t);
    private:
    };
}
