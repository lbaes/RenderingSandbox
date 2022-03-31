#include "GLRenderer.h"
#include "glad/glad.h"

void Eng::GLRenderer::RenderModel(const GPUModelHandle& model, Eng::Transform t) {
    _shader.use();
    _shader.uniform_set("model", t.GetTransformation());
    _shader.uniform_set("inverse_model", glm::inverse(t.GetTransformation()));
    _shader.uniform_set("view", _camera.GetView());
    _shader.uniform_set("projection", _camera.GetProjection());
	_shader.uniform_set("view_pos", _camera.GetPosition());
    for (const auto& mesh: model.meshes) {
        RenderMesh(mesh);
    }
}

void Eng::GLRenderer::RenderMesh(const GPUMeshHandle& mesh) {
    unsigned int diffuseNr  = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr   = 1;
    unsigned int heightNr   = 1;
    for (size_t i = 0; i < mesh.textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        std::string number;
        auto usage = mesh.textures[i].usage;
        std::string name;
        if(usage == Texture2DUsage::DIFFUSE) {
            name = "diffuse";
            number = std::to_string(diffuseNr++);
        }
        else if(usage == Texture2DUsage::SPECULAR) {
            name = "specular";
            number = std::to_string(specularNr++);
        }
        else if(usage == Texture2DUsage::NORMAL) {
            name = "normal";
            number = std::to_string(normalNr++);
        }
        else if(usage == Texture2DUsage::HEIGHT) {
            name = "height";
            number = std::to_string(heightNr++);
        }
        const auto uniform_name = "material." + (name + number);
        _shader.uniform_set(uniform_name, (int)i);
        glBindTexture(GL_TEXTURE_2D, mesh.textures[i].id);
    }
    glBindVertexArray(mesh.VAO);
    glDrawElements(GL_TRIANGLES, mesh.index_count, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}

void Eng::GLRenderer::RenderLine(const GPULineHandle &line, Color4 color) {
    _shader.use();
    _shader.uniform_set("model", Transform().GetTransformation());
    _shader.uniform_set("view", _camera.GetView());
    _shader.uniform_set("projection", _camera.GetProjection());
    _shader.uniform_set("color", {color.red(), color.green(), color.blue(), color.alpha()});
    glBindVertexArray(line.VAO);
    glDrawArrays(GL_LINES, 0, 2);
    glBindVertexArray(0);
}

void Eng::GLRenderer::SetShader(GPUShaderHandle shader) {
    _shader = shader;
}

void Eng::GLRenderer::SetCamera(const Eng::Camera &camera) {
    _camera = camera;
}

void Eng::GLRenderer::SetClearColor(Eng::Color4 color){
	_clearColor = color;
	glClearColor(color.red(), color.green(), color.blue(), color.alpha());
}

void Eng::GLRenderer::Clear(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
