#pragma once
#include "RenderDevice.h"
#include "Camera.h"
#include "Engine/Core/Types/Transform.h"
namespace Eng
{
	class Renderer
	{
	public:
		virtual void RenderModel(const GPUModelHandle& model, Transform t) = 0;
		virtual void SetShader(GPUShaderHandle shader) = 0;
		virtual void SetCamera(const Camera& camera) = 0;
	};
}
