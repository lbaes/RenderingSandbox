#include "../RenderDevice.h"
#include "Engine/Resources/Texture2D.h"
#include "Engine/Resources/Mesh.h"
#include "Engine/Resources/Model.h"
#include "Engine/Resources/Shader.h"
#include "Engine/LLRenderer/ErrorLogger.h"
#include "glad/glad.h"
#include <stdexcept>
#include <sstream>
#include <span>
#include <cassert>

namespace Eng {

    enum class VERTEX_BUFFER_USAGE {
        STATIC,
        DYNAMIC
    };

    struct BufferHandles {
        GLuint VAO = 0;
        GLuint VBO = 0;
        GLuint EBO = 0;
    };

    struct Targets {
        GLenum format;
        GLint internalFormat;
    };

    constexpr GLenum GetGLUsage(VERTEX_BUFFER_USAGE usage);

    constexpr Targets GetTargets(Texture2DUsage usage, int channels);

    BufferHandles CreateBuffers(const std::vector<Vertex> &vertexBuffer, const std::vector<GLuint> &indexBuffer);

    unsigned int CreateFrameBuffer();

    int CheckCompileErrors(unsigned int shader, const std::string &shaderType);

    int CheckLinkerErrors(unsigned int shader);

    void RenderDevice::InitRenderDevice() {
        logger = Logger::GetLogger();
#ifdef ENGINE_DEBUG
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(ErrorLogger::gl_error_logger, nullptr);
#endif // ENGINE_DEBUG
        // reinterpret_cast<const char*> to get rid of fmt warning
        logger->LogInfo("Vendor: {}", reinterpret_cast<const char *>(glGetString(GL_VENDOR)));
        logger->LogInfo("Renderer: {}", reinterpret_cast<const char *>(glGetString(GL_RENDERER)));
        logger->LogInfo("Version: {}", reinterpret_cast<const char *>(glGetString(GL_VERSION)));
        logger->LogInfo("GLSL Version: {}", reinterpret_cast<const char *>(glGetString(GL_SHADING_LANGUAGE_VERSION)));
        glEnable(GL_MULTISAMPLE);
        glEnable(GL_FRAMEBUFFER_SRGB);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        CreatePrimitiveVAOs();
        initialized = true;
    }

    /// <summary>
    /// Creates a 2D Texture resource on the GPU
    /// </summary>
    /// <param name="tex"></param>
    /// <returns>ID used to refer to the texture</returns>
    GPUTextureHandle RenderDevice::CreateTexture2D(const Texture2D &tex, Texture2DUsage usage) {
        logger->LogInfo("Loading texture {}", tex.GetPath());
        unsigned int texture_id = 0;
        glGenTextures(1, &texture_id);
        glBindTexture(GL_TEXTURE_2D, texture_id);

        assert(tex.GetData() != nullptr);
        assert(tex.GetWidth() == tex.GetHeight());
        Targets targets = GetTargets(usage, tex.GetNumberOfChannels());
        const int num_mipmap_level = int(floor(log2(fmax(tex.GetWidth(), tex.GetHeight()))) + 1);
        glTexStorage2D(GL_TEXTURE_2D, num_mipmap_level, targets.internalFormat, tex.GetWidth(), tex.GetHeight());
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, tex.GetWidth(), tex.GetHeight(), targets.format, GL_UNSIGNED_BYTE,
                        tex.GetData());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);

        GPUTextureHandle t;
        t.id = texture_id;
        t.usage = usage;
        loaded_textures.emplace(tex.GetPath(), t);
        logger->LogInfo("loaded texture {} with id: {}, usage: {} ", tex.GetPath(), t.id, static_cast<int>(t.usage));
        return t;
    }

    BufferHandles CreateBuffers(const std::vector<Vertex> &vertexBuffer, const std::vector<GLuint> &indexBuffer) {
        // Create Buffers
        unsigned int vbo_id, vao_id, ebo_id;
        glGenBuffers(1, &vbo_id);
        glGenBuffers(1, &ebo_id);
        glGenVertexArrays(1, &vao_id);

        // Bind VAO
        glBindVertexArray(vao_id);

        // Copy vertex data to VBO
        glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertexBuffer.size(), &vertexBuffer[0], GL_STATIC_DRAW);

        // Copy index data to EBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indexBuffer.size(), &indexBuffer[0],
                     GL_STATIC_DRAW);

        BufferHandles bh;
        bh.VAO = vao_id;
        bh.VBO = vbo_id;
        bh.EBO = ebo_id;
        return bh;
    }

    GPUShader RenderDevice::CreateShaderProgram(const Shader &vertexShader, const Shader &fragmentShader) {
        unsigned int vShaderID, fShaderID, programID;

        std::string fShaderCodeStr{fragmentShader.GetShaderSourceCodeComplete()};
        std::string vShaderCodeStr{vertexShader.GetShaderSourceCodeComplete()};

        const auto fShaderCode = fShaderCodeStr.c_str();
        const auto vShaderCode = vShaderCodeStr.c_str();

        // Vertex Shader
        logger->LogInfo("Creating vertex shader\n Definitions:\n {}", vertexShader.GetShaderDefines());
        vShaderID = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vShaderID, 1, &vShaderCode, NULL);
        glCompileShader(vShaderID);
        CheckCompileErrors(vShaderID, "VERTEX");

        // Fragment Shader
        logger->LogInfo("Creating fragment shader\n Definitions:\n {}", fragmentShader.GetShaderDefines());
        fShaderID = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fShaderID, 1, &fShaderCode, NULL);
        glCompileShader(fShaderID);
        CheckCompileErrors(fShaderID, "FRAGMENT");

        // Shader Program
        logger->LogInfo("Creating shader program");
        programID = glCreateProgram();
        glAttachShader(programID, vShaderID);
        glAttachShader(programID, fShaderID);
        glLinkProgram(programID);
        CheckLinkerErrors(programID);

        // Delete shaders
        glDeleteShader(vShaderID);
        glDeleteShader(fShaderID);

        GPUShader sh;
        sh.id = programID;
        sh.effects = vertexShader.GetShaderEffects() | fragmentShader.GetShaderEffects();
        logger->LogInfo("Created shader program with ID: {}", sh.id);
        return sh;
    }

    GPUMeshHandle RenderDevice::CreateMesh(const Mesh &mesh) {
        logger->LogInfo("Creating Mesh with:\n\t{} vertices\n\t{} indices\n\t{} textures",
                        mesh.vertices.size(), mesh.indices.size(), mesh.textures.size());
        // Create Buffers
        BufferHandles bh = CreateBuffers(mesh.vertices, mesh.indices);

        // vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) nullptr);

        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, vertex_normal));

        // vertex texture coordinates
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, texture_coordinate));

        // tangents
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, tangent));

        // unbind
        glBindVertexArray(0);

        GPUMeshHandle m;
        m.VAO = bh.VAO;
        m.index_count = static_cast<GLsizei>(mesh.indices.size());

        // Loads textures into GPU
        for (const auto &texture: mesh.textures) {
            if (!loaded_textures.contains(texture.file_path)) {
                Texture2D loaded_texture;
                loaded_texture.LoadFromFile(texture.file_path);
                auto tex_handle = CreateTexture2D(loaded_texture, texture.usage);
                tex_handle.usage = texture.usage;
                loaded_textures.emplace(texture.file_path, tex_handle);
                m.textures.push_back(tex_handle);
            } else {
                m.textures.push_back(loaded_textures.at(texture.file_path));
            }
        }
        return m;
    }

    GPUModelHandle RenderDevice::CreateModel(const Model &model) {
        logger->LogInfo("Creating model with\n\t{} meshes", model.GetMeshes().size());
        GPUModelHandle modelHandle;
        for (const auto &mesh: model.GetMeshes()) {
            auto mesh_handle = CreateMesh(mesh);
            modelHandle.meshes.push_back(mesh_handle);
        }
        return modelHandle;
    }

    GPULineHandle RenderDevice::CreateLine(const Line &line) {

        // Create buffers
        unsigned int vbo_id, vao_id;
        glGenBuffers(1, &vbo_id);
        glGenVertexArrays(1, &vao_id);

        // Bind VAO
        glBindVertexArray(vao_id);

        // Copy vertex data to VBO
        glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Line), &line, GL_STATIC_DRAW);

        // Bind attributes
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(line.p1), (void *) 0);

        // Unbind
        glBindVertexArray(0);

        GPULineHandle lineHandle;
        lineHandle.VAO = vao_id;
        return lineHandle;
    }

    GPURenderTarget RenderDevice::CreateRenderTarget(int width, int height,
                                                     unsigned int attachments,
                                                     GPURenderTargetOptions options) {
        GPURenderTarget renderTarget;
        renderTarget.width = width;
        renderTarget.height = height;
        unsigned int FBO;
        glGenFramebuffers(1, &FBO);
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);

        if (attachments & GPURenderTargetAttachments::COLOR) {
            unsigned int color;
            glGenTextures(1, &color);
            glBindTexture(GL_TEXTURE_2D, color);
            if (options & GPURenderTargetOptions::SRGB_COLOR)
                glTexStorage2D(GL_TEXTURE_2D, 1, GL_SRGB8, width, height);
            else
                glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, width, height);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color, 0);
            renderTarget.COLOR_ID = color;
        }

        if (attachments & GPURenderTargetAttachments::DEPTH) {
            unsigned int depth;
            glGenTextures(1, &depth);
            glBindTexture(GL_TEXTURE_2D, depth);
            glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32F, width, height);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth, 0);
            renderTarget.DEPTH_ID = depth;
        }

        auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        assert(status == GL_FRAMEBUFFER_COMPLETE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        renderTarget.ID = FBO;
        renderTarget.attachments = attachments;
        renderTarget.isSRGB = options & GPURenderTargetOptions::SRGB_COLOR;
        return renderTarget;
    }

    unsigned int RenderDevice::GetQuadVAO() const {
        return quad_vao;
    }

    void RenderDevice::CreatePrimitiveVAOs() {
        const static float vertices[] = {
                1.0f, 1.0f, 1.0f, 1.0f,
                1.0f, -1.0f, 1.0f, 0.0f,
                -1.0f, -1.0f, 0.0f, 0.0f,
                -1.0f, 1.0f, 0.0f, 1.0f
        };

        const static unsigned int indices[] = {
                0, 1, 3,
                1, 2, 3
        };

        GLuint VAO, VBO, EBO;
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        glGenVertexArrays(1, &VAO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) nullptr);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) (2 * sizeof(float)));
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        quad_vao = VAO;
    }

    constexpr GLenum GetGLUsage(VERTEX_BUFFER_USAGE usage) {
        switch (usage) {
            case Eng::VERTEX_BUFFER_USAGE::DYNAMIC:
                return GL_DYNAMIC_DRAW;
            case Eng::VERTEX_BUFFER_USAGE::STATIC:
            default:
                return GL_STATIC_DRAW;
        }
    }

    // TODO: MOVE TO SHADER COMPILER CLASS
    int CheckCompileErrors(unsigned int shader, const std::string &shaderType) {
        int compilationSuccess;
        char infoLog[512];

        glGetShaderiv(shader, GL_COMPILE_STATUS, &compilationSuccess);
        if (!compilationSuccess) {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::stringstream s;
            s << "ERROR::SHADER::" << shaderType << "::COMPILATION_FAILED\n" << infoLog << '\n';
            throw std::runtime_error(s.str());
        }

        return compilationSuccess;
    }

    // TODO: MOVE TO SHADER COMPILER CLASS
    int CheckLinkerErrors(unsigned int shader) {
        int linkSuccess;
        char infoLog[512];

        glGetProgramiv(shader, GL_LINK_STATUS, &linkSuccess);
        if (!linkSuccess) {
            glGetProgramInfoLog(shader, 512, nullptr, infoLog);
            std::stringstream s;
            s << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
            throw std::runtime_error(s.str());
        }

        return linkSuccess;
    }

    constexpr Targets GetTargets(Texture2DUsage usage, int channels) {
        Targets t{};
        switch (channels) {
            case 1:
                t.format = GL_RED;
                t.internalFormat = GL_R8;
                break;
            case 2:
                t.format = GL_RG;
                t.internalFormat = GL_RG8;
                break;
            case 3:
                t.format = GL_RGB;
                if (usage == Texture2DUsage::DIFFUSE) {
                    t.internalFormat = GL_SRGB8;
                } else {
                    t.internalFormat = GL_RGB8;
                }
                break;
            case 4:
                t.format = GL_RGBA;
                if (usage == Texture2DUsage::DIFFUSE)
                    t.internalFormat = GL_SRGB8_ALPHA8;
                else
                    t.internalFormat = GL_RGBA8;
                break;
            default:
                break;
        }
        return t;
    }
}