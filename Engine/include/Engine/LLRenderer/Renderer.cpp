#include "Renderer.h"
#include "glad/glad.h"

void Eng::Renderer::RenderModel(const GPUModelHandle& model, Eng::Transform t) {
    _shader.use();
    _shader.uniform_set("model", t.GetTransformation());
    _shader.uniform_set("view", _camera.GetView());
    _shader.uniform_set("projection", _camera.GetProjection());
    for (const auto& mesh: model.meshes) {
        RenderMesh(mesh);
    }
}

void Eng::Renderer::RenderMesh(const GPUMeshHandle& mesh) {
    unsigned int diffuseNr  = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr   = 1;
    unsigned int heightNr   = 1;
    for (unsigned int i = 0; i < mesh.textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        std::string number;
        auto usage = mesh.textures[i].usage;
        std::string name;
        if(usage == Texture2DUsage::DIFFUSE) {
            name = "texture_diffuse";
            number = std::to_string(diffuseNr++);
        }
        else if(usage == Texture2DUsage::SPECULAR) {
            name = "texture_specular";
            number = std::to_string(specularNr++);
        }
        else if(usage == Texture2DUsage::NORMAL) {
            name = "texture_normal";
            number = std::to_string(normalNr++);
        }
        else if(usage == Texture2DUsage::HEIGHT) {
            name = "texture_height";
            number = std::to_string(heightNr++);
        }
        _shader.uniform_set(name + number, (int)i);
        glBindTexture(GL_TEXTURE_2D, static_cast<int>(mesh.textures[i].id));
    }
    glBindVertexArray(mesh.VAO);
    glDrawElements(GL_TRIANGLES, mesh.index_count, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}

void Eng::Renderer::SetShader(GPUShaderHandle shader) {
    _shader = shader;
}

void Eng::Renderer::SetCamera(const Eng::Camera &camera) {
    _camera = camera;
}
