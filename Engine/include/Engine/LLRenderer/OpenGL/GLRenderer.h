#pragma once
#include "Engine/LLRenderer/Renderer.h"
#include "Engine/Core/Types/Transform.h"
#include "Engine/Core/Types/Color.h"
#include "Engine/LLRenderer/Camera.h"
#include "Engine/LLRenderer/RenderDevice.h"
#include "Engine/LLRenderer/PointLight.h"

namespace Eng {
    class GLRenderer : public Renderer{
    public:
		GLRenderer(RenderDevice* renderDevice, int width, int height);
		void RenderTexture2D(const GPUTextureHandle& handle, GPUQuad quad) override;
        void RenderModel(const GPUModelHandle& model, Transform t) override;
        void RenderDebugLine(const GPULineHandle& line, Color4 color) override;
        void RenderMesh(const GPUMeshHandle& mesh);
        void SetShader(GPUShader& shader) override;
        void SetCamera(const Camera& camera) override;
		void SetClearColor(Color4 color) override;
		void SetStaticPointLights(const std::vector<PointLight>& lights) override;
		void AddStaticPointLight(PointLight light) override;
		void SetDirectionalLight(DirectionalLight light) override;
		void Clear() override;
	private:
		RenderDevice* render_device;
        GPUShader* _shader;
        GPUShader _screenShader;
        GPUShader _modelShader;
		Camera _camera;
		std::vector<PointLight> _dynamicPointLights;
		std::vector<PointLight> _staticPointLights;
		DirectionalLight _directionalLight{};
		int screenWidth, screenHeight;
        GLuint quad_vao;
		const int max_lights = 10;
		void UpdateShaderPointLightUniforms(bool dynamic = true, bool use_shader = true);
		void UpdateShaderDirectionalLightUniforms(bool use_shader = true);
		void UpdateShaderMVPUniforms(Eng::Transform t, bool use_shader = true);
	};
}
