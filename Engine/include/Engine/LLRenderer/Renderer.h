#pragma once
#include "RenderDevice.h"
#include "Camera.h"
#include "Engine/Core/Types/Transform.h"
#include "Engine/Core/Types/Transform.h"
#include "Engine/Core/Types/Color.h"

namespace Eng
{
	class Renderer
	{
	public:
		virtual void RenderModel(const GPUModelHandle& model, Transform t) = 0;
        virtual void RenderLine(const GPULineHandle& line, Color4 color) = 0;
		virtual void SetShader(GPUShaderHandle shader) = 0;
		virtual void SetCamera(const Camera& camera) = 0;
		virtual void SetClearColor(Color4 color) = 0;
		virtual void Clear() = 0;
		virtual ~Renderer() = default;
	};
}
