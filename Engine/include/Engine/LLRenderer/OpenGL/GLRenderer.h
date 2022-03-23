#pragma once
#include "Engine/LLRenderer/Renderer.h"
#include "Engine/Core/Types/Transform.h"
#include "Engine/LLRenderer/Camera.h"
#include "Engine/LLRenderer/RenderDevice.h"

namespace Eng {
    class GLRenderer : public Renderer{
    public:
        void RenderModel(const GPUModelHandle& model, Transform t) override;
        void RenderLine(const GPULineHandle& line) override;
        void RenderMesh(const GPUMeshHandle& mesh);
        void SetShader(GPUShaderHandle shader) override;
        void SetCamera(const Camera& camera) override;
    private:
        GPUShaderHandle _shader;
        Camera _camera;
    };
}
