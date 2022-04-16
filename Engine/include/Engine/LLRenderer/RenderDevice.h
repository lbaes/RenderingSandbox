#pragma once
#if defined(ENG_WINDOWS) || defined(ENG_LINUX)
    #include "OpenGL/GLTexture.h"
    #include "OpenGL/GLShader.h"
    #include "OpenGL/GLMesh.h"
    #include "OpenGL/GLModel.h"
    #include "OpenGL/GLLine.h"
	#include "OpenGL/GLFramebuffer.h"
	#include "OpenGL/GLQuad.h"
using GPUTextureHandle = Eng::ogl::GLTexture;
    using GPUShader = Eng::ogl::GLShader;
    using GPUMeshHandle = Eng::ogl::GLMesh;
    using GPUModelHandle = Eng::ogl::GLModel;
    using GPULineHandle = Eng::ogl::GLLine;
	using GPURenderTarget = Eng::ogl::GLFramebuffer;
	using GPURenderTargetAttachments = Eng::ogl::GLFrameBufferAttachments;
	using GPURenderTargetOptions = Eng::ogl::GLFrameBufferOptions;
	using GPUQuad = Eng::ogl::GLQuad;
#endif
#include <unordered_map>
#include <string>
#include "Engine/Core/Logger.h"
#include "Engine/Core/Types/Line.h"

namespace Eng {
	class Texture2D;
	class Shader;
	class Mesh;
	class Model;
	class RenderDevice {
	public:
		void InitRenderDevice();
		GPUTextureHandle CreateTexture2D(const Texture2D& tex, Texture2DUsage usage = Texture2DUsage::DIFFUSE);
		GPUShader CreateShaderProgram(const Shader& vertexShader, const Shader& fragmentShader);
		GPUMeshHandle CreateMesh(const Mesh& mesh);
		GPUModelHandle CreateModel(const Model& model);
		GPURenderTarget CreateRenderTarget(int width, int height, GPURenderTargetAttachments attachments = GPURenderTargetAttachments::COLOR, GPURenderTargetOptions options = static_cast<GPURenderTargetOptions>(0u));
		GPULineHandle CreateLine(const Line& line);
        unsigned int GetQuadVAO() const;
	private:
		bool initialized = false;
        std::unordered_map<std::string, GPUTextureHandle> loaded_textures;
        Logger* logger = nullptr;
        unsigned int quad_vao = 0;
        void CreatePrimitiveVAOs();
    };
}