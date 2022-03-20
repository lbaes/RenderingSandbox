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
#ifdef ENGINE_DEBUG
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(ErrorLogger::gl_error_logger, nullptr);
#endif // ENGINE_DEBUG
        initialized = true;
    }

    /// <summary>
    /// Creates a 2D Texture resource on the GPU
    /// </summary>
    /// <param name="tex"></param>
    /// <returns>ID used to refer to the texture</returns>
    GPUTextureHandle RenderDevice::CreateTexture2D(const Texture2D& tex)
    {
        unsigned int texture_id;
        glGenTextures(1, &texture_id);
        glBindTexture(GL_TEXTURE_2D, texture_id);

        // TODO: make parameters configurable
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Copy bytes to opengl texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex.GetWidth(), tex.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.GetData());

        // Mipmapping
        glGenerateMipmap(GL_TEXTURE_2D);

        GPUTextureHandle t;
        t.id = texture_id;
        loaded_textures.emplace(tex.GetPath(), t);
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
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertexBuffer.size(), vertexBuffer.data(), GL_STATIC_DRAW);

        // Copy index data to EBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indexBuffer.size(), indexBuffer.data(), GL_STATIC_DRAW);

        // Unbind VAO
        glBindVertexArray(0);

        // Unbind buffers
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

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
        vShaderID = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vShaderID, 1, &vShaderCode, NULL);
        glCompileShader(vShaderID);
        CheckCompileErrors(vShaderID, "VERTEX");

        // Fragment Shader
        fShaderID = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fShaderID, 1, &fShaderCode, NULL);
        glCompileShader(fShaderID);
        CheckCompileErrors(fShaderID, "FRAGMENT");

        // Shader Program
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

        return sh;
    }

    GPUMeshHandle RenderDevice::CreateMesh(const Mesh& mesh)
    {
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

        // Loads textures into GPU
        for (const auto& texture : mesh.textures) {
            if (!loaded_textures.contains(texture.file_path)){
                Texture2D loaded_texture;
                loaded_texture.LoadFromFile(texture.file_path);
                auto tex_handle = CreateTexture2D(loaded_texture);
                loaded_textures.emplace(texture.file_path, tex_handle);
                m.textures.push_back(tex_handle);
            }else{
                m.textures.push_back(loaded_textures.at(texture.file_path));
            }
        }
        return m;
    }

    GPUModelHandle RenderDevice::CreateModel(const Model& model) {
        GPUModelHandle modelHandle;
        for (const auto& mesh : model.GetMeshes()) {
            auto mesh_handle = CreateMesh(mesh);
            modelHandle.meshes.push_back(mesh_handle);
        }
        return modelHandle;
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