#pragma once
#if defined(ENG_WINDOWS) || defined(ENG_LINUX)
    #include "OpenGL/GLTexture.h"
    #include "OpenGL/GLShader.h"
    #include "OpenGL/GLMesh.h"
    #include "OpenGL/GLModel.h"
    #include "OpenGL/GLLine.h"
    using GPUTextureHandle = Eng::ogl::GLTexture;
    using GPUShaderHandle = Eng::ogl::GLShader;
    using GPUMeshHandle = Eng::ogl::GLMesh;
    using GPUModelHandle = Eng::ogl::GLModel;
    using GPULineHandle = Eng::ogl::GLLine;
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
		GPUTextureHandle CreateTexture2D(const Texture2D& tex);
		GPUShaderHandle CreateShaderProgram(const Shader& vertexShader, const Shader& fragmentShader);
		GPUMeshHandle CreateMesh(const Mesh& mesh);
		GPUModelHandle CreateModel(const Model& model);
		GPULineHandle CreateLine(const Line& line);
	private:
		bool initialized = false;
        std::unordered_map<std::string, GPUTextureHandle> loaded_textures;
        Logger* logger = nullptr;
	};
}