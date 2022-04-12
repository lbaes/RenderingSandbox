#pragma once
#include "RenderDevice.h"
#include "Engine/Core/Types/Transform.h"
#include "Engine/Core/Types/Transform.h"
#include "Engine/Core/Types/Color.h"
#include "PointLight.h"
#include "DirectionalLight.h"

namespace Eng
{
	class Camera;
	class Renderer
	{
	public:
		virtual void RenderModel(const GPUModelHandle& model, Transform t) = 0;
        virtual void RenderDebugLine(const GPULineHandle& line, Color4 color) = 0;
		virtual void SetShader(GPUShader& shader) = 0;
		virtual void SetCamera(const Camera& camera) = 0;
		virtual void SetClearColor(Color4 color) = 0;
		virtual void Clear() = 0;
		virtual void SetDirectionalLight(DirectionalLight light) = 0;
		virtual void SetStaticPointLights(const std::vector<PointLight>& lights) = 0;
		virtual void AddStaticPointLight(PointLight light) = 0;
		virtual ~Renderer() = default;
	};
}
