#include "GLRenderer.h"
#include "glad/glad.h"
#include "Engine/Resources/Shader.h"
#include "Engine/Resources/Mesh.h"

Eng::GLRenderer::GLRenderer(Eng::RenderDevice *renderDevice, int width, int height)
        : Renderer(), render_device{renderDevice}, screenWidth{width}, screenHeight{height}, _shader{},
          _directionalLight{} {
    _staticPointLights.reserve(max_lights);
    _dynamicPointLights.reserve(max_lights);
    Eng::Shader screenVertex, screenFragment;
    screenVertex.LoadFromDisk("resources/vScreen.glsl");
    screenFragment.LoadFromDisk("resources/fScreen.glsl");
    _screenShader = renderDevice->CreateShaderProgram(screenVertex, screenFragment);
}

void Eng::GLRenderer::RenderModel(const GPUModelHandle &model, Eng::Transform t) {
    _shader->use();
    UpdateShaderMVPUniforms(t, false);
    UpdateShaderPointLightUniforms(true, false);
    UpdateShaderDirectionalLightUniforms(false);
    for (const auto &mesh: model.meshes) {
        RenderMesh(mesh);
    }
}

void Eng::GLRenderer::RenderMesh(const GPUMeshHandle &mesh) {
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    for (unsigned int i = 0; i < mesh.textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        std::string number;
        auto usage = mesh.textures[i].usage;
        std::string name;
        if (usage == Texture2DUsage::DIFFUSE) {
            name = "diffuse";
            number = std::to_string(diffuseNr++);
        } else if (usage == Texture2DUsage::SPECULAR) {
            name = "specular";
            number = std::to_string(specularNr++);
        } else if (usage == Texture2DUsage::NORMAL) {
            name = "normal";
            number = std::to_string(normalNr++);
        }
        const auto uniform_name = "material." + (name + number);
        _shader->uniform_set(uniform_name, (int) i);
        glBindTexture(GL_TEXTURE_2D, mesh.textures[i].id);
    }
    glBindVertexArray(mesh.VAO);
    glDrawElements(GL_TRIANGLES, mesh.index_count, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}

void Eng::GLRenderer::RenderDebugLine(const GPULineHandle &line, Color4 color) {
    _shader->use();
    _shader->uniform_set("model", Transform().GetTransformation());
    _shader->uniform_set("view", _camera.GetView());
    _shader->uniform_set("projection", _camera.GetProjection());
    _shader->uniform_set("color", {color.red(), color.green(), color.blue(), color.alpha()});
    glBindVertexArray(line.VAO);
    glDrawArrays(GL_LINES, 0, 2);
    glBindVertexArray(0);
}

void Eng::GLRenderer::SetShader(GPUShader &shader) {
    _shader = &shader;
    UpdateShaderPointLightUniforms(false, true);
}

void Eng::GLRenderer::SetCamera(const Eng::Camera &camera) {
    _camera = camera;
}

void Eng::GLRenderer::SetClearColor(Eng::Color4 color) {
    glClearColor(color.red(), color.green(), color.blue(), color.alpha());
}

void Eng::GLRenderer::Clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Eng::GLRenderer::SetStaticPointLights(const std::vector<PointLight> &lights) {
    if (lights.size() <= max_lights) {
        _staticPointLights = lights;
        UpdateShaderPointLightUniforms(false, true);
    }
}

void Eng::GLRenderer::AddStaticPointLight(Eng::PointLight light) {
    if (_staticPointLights.size() < max_lights) {
        _staticPointLights.push_back(light);
        UpdateShaderPointLightUniforms(false, true);
    }
}

void Eng::GLRenderer::SetDirectionalLight(Eng::DirectionalLight light) {
    _directionalLight = light;
}

void Eng::GLRenderer::UpdateShaderPointLightUniforms(bool dynamic, bool use_shader) {
    auto &pointLights = (dynamic ? _dynamicPointLights : _staticPointLights);
    if (use_shader) _shader->use();
    for (int i = 0; i < pointLights.size(); ++i) {
        _shader->uniform_set_structArray_member("light", i, "ambient", pointLights[i].ambient);
        _shader->uniform_set_structArray_member("light", i, "diffuse", pointLights[i].diffuse);
        _shader->uniform_set_structArray_member("light", i, "specular", pointLights[i].specular);
        _shader->uniform_set_structArray_member("light", i, "constant", pointLights[i].constant);
        _shader->uniform_set_structArray_member("light", i, "linear", pointLights[i].linear);
        _shader->uniform_set_structArray_member("light", i, "quadratic", pointLights[i].quadratic);
        if (_shader->effects & Eng::ShaderEffects::NORMAL_MAP)
            _shader->uniform_set_array("light_pos", i, pointLights[i].position);
        else
            _shader->uniform_set_structArray_member("light", i, "position", pointLights[i].position);
    }

    // Set contribution of remaining lights to zero;
    const int lights_in_use = static_cast<int>(_staticPointLights.size() + _dynamicPointLights.size());
    for (int i = lights_in_use; i < max_lights; ++i) {
        _shader->uniform_set_structArray_member("light", i, "ambient", Vec3{0.0f});
        _shader->uniform_set_structArray_member("light", i, "diffuse", Vec3{0.0f});
        _shader->uniform_set_structArray_member("light", i, "specular", Vec3{0.0f});
        _shader->uniform_set_structArray_member("light", i, "constant", 1.0f);
        _shader->uniform_set_structArray_member("light", i, "linear", 1.0f);
        _shader->uniform_set_structArray_member("light", i, "quadratic", 1.0f);
        if (_shader->effects & Eng::ShaderEffects::NORMAL_MAP)
            _shader->uniform_set_array("light_pos", i, Vec3{0.0f});
        else
            _shader->uniform_set_structArray_member("light", i, "position", Vec3{0.0f});
    }
}

void Eng::GLRenderer::UpdateShaderDirectionalLightUniforms(bool use_shader) {
    if (use_shader) _shader->use();
    _shader->uniform_set("directional_light.direction", _directionalLight.direction);
    _shader->uniform_set("directional_light.ambient", _directionalLight.ambient);
    _shader->uniform_set("directional_light.diffuse", _directionalLight.diffuse);
    _shader->uniform_set("directional_light.specular", _directionalLight.specular);
}

void Eng::GLRenderer::UpdateShaderMVPUniforms(Eng::Transform t, bool use_shader) {
    if (use_shader) _shader->use();
    _shader->uniform_set("model", t.GetTransformation());
    _shader->uniform_set("inverse_model", glm::inverse(Mat3(t.GetTransformation())));
    _shader->uniform_set("view", _camera.GetView());
    _shader->uniform_set("projection", _camera.GetProjection());
    _shader->uniform_set("view_pos", _camera.GetPosition());
}

void Eng::GLRenderer::RenderTexture2D(const GPUTextureHandle& handle, GPUQuad quad) {
    Mat4 ortho = glm::ortho(0.0f, float(screenWidth), float(screenHeight), 0.0f, -1000.0f, 1000.0f);
    Mat4 model = Mat4(1.0f);
    model = glm::translate(model, Vec3{quad.x, quad.y, 0.0f});
    model = glm::scale(model, Vec3(quad.w, quad.h, 0.0f));
    _screenShader.use();
    _screenShader.uniform_set("model", model);
    _screenShader.uniform_set("projection", ortho);
    _screenShader.uniform_set("screenTexture", int(0));
    glDisable(GL_DEPTH_TEST);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, handle.id);
    glBindVertexArray(render_device->GetQuadVAO());
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    glEnable(GL_DEPTH_TEST);
}

void Eng::GLRenderer::SetRenderTarget(GPURenderTarget& target) {
    glBindFramebuffer(GL_FRAMEBUFFER, target.ID);
    glViewport(0, 0, target.width, target.height);
}

void Eng::GLRenderer::SetRenderTarget() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, screenWidth, screenHeight);
}

void Eng::GLRenderer::RenderTexture2D(const GPURenderTarget &handle, GPUQuad quad) {
    GPUTextureHandle th;
    th.id = handle.COLOR_ID;
    th.usage = Texture2DUsage::DIFFUSE;
    RenderTexture2D(th, quad);
}
