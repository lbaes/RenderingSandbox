#include "../RenderDevice.h"
#include "Engine/Resources/Texture2D.h"
#include "Engine/Resources/Mesh.h"
#include "Engine/Resources/Model.h"
#include "Engine/Resources/Shader.h"
#include "Engine/LLRenderer/ErrorLogger.h"
#include "glad/glad.h"

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

    constexpr GLenum GetGLUsage(VERTEX_BUFFER_USAGE usage);
    BufferHandles CreateBuffers(const std::vector<Vertex>& vertexBuffer, const std::vector<GLuint>& indexBuffer);
    int CheckCompileErrors(unsigned int shader, const std::string& shaderType);
    int CheckLinkerErrors(unsigned int shader);

    void RenderDevice::InitRenderDevice()
    {
        logger = Logger::GetLogger();
#ifdef ENGINE_DEBUG
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(ErrorLogger::gl_error_logger, nullptr);
        // reinterpret_cast<const char*> to get rid of fmt warning
        logger->LogInfo("Vendor: {}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
        logger->LogInfo("Renderer: {}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
        logger->LogInfo("Version: {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
        logger->LogInfo("GLSL Version: {}", reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)));
#endif // ENGINE_DEBUG
        glEnable(GL_DEPTH_TEST);
        initialized = true;
    }

    /// <summary>
    /// Creates a 2D Texture resource on the GPU
    /// </summary>
    /// <param name="tex"></param>
    /// <returns>ID used to refer to the texture</returns>
    GPUTextureHandle RenderDevice::CreateTexture2D(const Texture2D& tex)
    {
        logger->LogInfo("Loading texture {}", tex.GetPath());
        unsigned int texture_id = 0;
        glGenTextures(1, &texture_id);
        glBindTexture(GL_TEXTURE_2D, texture_id);

        // Copy bytes to opengl texture
        assert(tex.GetData() != nullptr);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex.GetWidth(), tex.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.GetData());

        // Mipmaps
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        GPUTextureHandle t;
        t.id = texture_id;
        loaded_textures.emplace(tex.GetPath(), t);
        logger->LogInfo("loaded texture {} with id: {}", tex.GetPath(), t.id);
        return t;
    }

    BufferHandles CreateBuffers(const std::vector<Vertex>& vertexBuffer, const std::vector<GLuint>& indexBuffer)
    {
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
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indexBuffer.size(), &indexBuffer[0], GL_STATIC_DRAW);

        BufferHandles bh;
        bh.VAO = vao_id;
        bh.VBO = vbo_id;
        bh.EBO = ebo_id;
        return bh;
    }

    GPUShaderHandle RenderDevice::CreateShaderProgram(const Shader& vertexShader, const Shader& fragmentShader)
    {
        unsigned int vShaderID, fShaderID, programID;
        const auto fShaderCode = fragmentShader.GetShaderSource().c_str();
        const auto vShaderCode = vertexShader.GetShaderSource().c_str();

        // Vertex Shader
        logger->LogInfo("Creating vertex shader");
        vShaderID = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vShaderID, 1, &vShaderCode, NULL);
        glCompileShader(vShaderID);
        CheckCompileErrors(vShaderID, "VERTEX");

        // Fragment Shader
        logger->LogInfo("Creating fragment shader");
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

        GPUShaderHandle sh;
        sh.id = programID;
        logger->LogInfo("Created shader program with ID: {}", sh.id);
        return sh;
    }

    GPUMeshHandle RenderDevice::CreateMesh(const Mesh& mesh)
    {
        logger->LogInfo("Creating Mesh with:\n\t{} vertices\n\t{} indices\n\t{} textures",
                        mesh.vertices.size(), mesh.indices.size(), mesh.textures.size());
        // Create Buffers
        BufferHandles bh = CreateBuffers(mesh.vertices, mesh.indices);

        // vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, vertex_normal));

        // vertex texture coordinates
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture_coordinate));

        // unbind
        glBindVertexArray(0);

        GPUMeshHandle m;
        m.VAO = bh.VAO;
        m.VBO = bh.VBO;
        m.EBO = bh.EBO;
        m.index_count = static_cast<GLsizei>(mesh.indices.size());

        // Loads textures into GPU
        for (const auto& texture : mesh.textures) {
            if (!loaded_textures.contains(texture.file_path)){
                Texture2D loaded_texture;
                loaded_texture.LoadFromFile(texture.file_path);
                auto tex_handle = CreateTexture2D(loaded_texture);
                tex_handle.usage = texture.usage;
                loaded_textures.emplace(texture.file_path, tex_handle);
                m.textures.push_back(tex_handle);
            }else{
                m.textures.push_back(loaded_textures.at(texture.file_path));
            }
        }
        return m;
    }

    GPUModelHandle RenderDevice::CreateModel(const Model& model) {
        logger->LogInfo("Creating model with\n\t{} meshes", model.GetMeshes().size());
        GPUModelHandle modelHandle;
        for (const auto& mesh : model.GetMeshes()) {
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
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(line.p1), (void*)0);

        // Unbind
        glBindVertexArray(0);

        GPULineHandle lineHandle;
        lineHandle.VAO = vao_id;
        return lineHandle;
    }

    constexpr GLenum GetGLUsage(VERTEX_BUFFER_USAGE usage) {
        switch (usage)
        {
            case Eng::VERTEX_BUFFER_USAGE::DYNAMIC:
                return GL_DYNAMIC_DRAW;
                break;
            case Eng::VERTEX_BUFFER_USAGE::STATIC:
                return GL_STATIC_DRAW;
                break;
            default:
                return GL_STATIC_DRAW;
                break;
        }
    }

    // TODO: MOVE TO SHADER COMPILER CLASS
    int CheckCompileErrors(unsigned int shader, const std::string& shaderType) {
        int compilationSuccess;
        char infoLog[512];

        glGetShaderiv(shader, GL_COMPILE_STATUS, &compilationSuccess);
        if (!compilationSuccess)
        {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::" << shaderType << "::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        return compilationSuccess;
    }

    // TODO: MOVE TO SHADER COMPILER CLASS
    int CheckLinkerErrors(unsigned int shader) {
        int linkSuccess;
        char infoLog[512];

        glGetProgramiv(shader, GL_LINK_STATUS, &linkSuccess);
        if (!linkSuccess)
        {
            glGetProgramInfoLog(shader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }

        return linkSuccess;
    }
}