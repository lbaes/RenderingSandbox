#pragma once
#include <vector>
#include <glad/glad.h>
namespace Eng::ogl {
    struct GLMesh {
        GLuint VAO;
        GLsizei index_count;
        std::vector<GLTexture> textures;
    };
}