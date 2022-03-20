#pragma once
#include "Engine/Core/Types/Transform.h"
#include "Engine/LLRenderer/OpenGL/Camera.h"
#include "RenderDevice.h"

namespace Eng {
    class Renderer {
    public:
        void RenderModel(const GPUModelHandle& model, Transform t);
        void RenderMesh(const GPUMeshHandle& mesh);
        void SetShader(GPUShaderHandle shader);
        void SetCamera(const Camera& camera);
    private:
        GPUShaderHandle _shader;
        Camera _camera;
    };
}
