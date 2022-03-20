#pragma once
#include <vector>
#include <glad/glad.h>
namespace Eng::ogl {
    struct GLMesh {
        GLuint VAO;
        GLuint VBO;
        GLuint EBO;
        std::vector<GLTexture> textures;
    };
}