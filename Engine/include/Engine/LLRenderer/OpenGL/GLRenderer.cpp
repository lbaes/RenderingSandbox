#include "GLRenderer.h"
#include "glad/glad.h"
#include "Engine/Resources/Shader.h"

void UpdateShaderLightUniforms(Eng::ogl::GLShader shader, std::vector<Eng::PointLight>& pointLights){
	shader.use();
	for (int i = 0; i < pointLights.size(); ++i)
	{
		shader.uniform_set_structArray_member("light", i, "ambient", pointLights[i].ambient);
		shader.uniform_set_structArray_member("light", i, "diffuse", pointLights[i].diffuse);
		shader.uniform_set_structArray_member("light", i, "specular", pointLights[i].specular);
		shader.uniform_set_structArray_member("light", i, "constant", pointLights[i].constant);
		shader.uniform_set_structArray_member("light", i, "linear", pointLights[i].linear);
		shader.uniform_set_structArray_member("light", i, "quadratic", pointLights[i].quadratic);
		if (shader.effects & Eng::ShaderEffects::NORMAL_MAP)
			shader.uniform_set_array("light_pos", i, pointLights[i].position);
		else
			shader.uniform_set_structArray_member("light", i, "position", pointLights[i].position);
	}
}

void Eng::GLRenderer::RenderModel(const GPUModelHandle& model, Eng::Transform t) {
    _shader.use();
    _shader.uniform_set("model", t.GetTransformation());
    _shader.uniform_set("inverse_model", glm::inverse(Mat3(t.GetTransformation())));
	_shader.uniform_set("view", _camera.GetView());
	_shader.uniform_set("projection", _camera.GetProjection());
	_shader.uniform_set("view_pos", _camera.GetPosition());
	_shader.uniform_set("directional_light.direction", Vec3{0.0f, -1.0f, 0.0f});
	_shader.uniform_set("directional_light.ambient", Vec3{0.1f});
	_shader.uniform_set("directional_light.diffuse", Vec3{0.3f});
	_shader.uniform_set("directional_light.specular", Vec3{0.2f});

	UpdateShaderLightUniforms(_shader, _staticPointLights);
	UpdateShaderLightUniforms(_shader, _dynamicPointLights);

	const int max_lights = 10;
	const int lights_in_use = _staticPointLights.size() + _dynamicPointLights.size();
	for (int i = lights_in_use; i < max_lights ; ++i)
	{
		_shader.uniform_set_structArray_member("light", i, "ambient", Vec3{0.0f});
		_shader.uniform_set_structArray_member("light", i, "diffuse", Vec3{0.0f});
		_shader.uniform_set_structArray_member("light", i, "specular",Vec3{0.0f});
		_shader.uniform_set_structArray_member("light", i, "constant", 1.0f);
		_shader.uniform_set_structArray_member("light", i, "linear", 1.0f);
		_shader.uniform_set_structArray_member("light", i, "quadratic", 1.0f);
		if (_shader.effects & Eng::ShaderEffects::NORMAL_MAP)
			_shader.uniform_set_array("light_pos", i, Vec3{0.0f});
		else
			_shader.uniform_set_structArray_member("light", i, "position", Vec3{0.0f});
	}

    for (const auto& mesh: model.meshes) {
        RenderMesh(mesh);
    }
}

void Eng::GLRenderer::RenderMesh(const GPUMeshHandle& mesh) {
    unsigned int diffuseNr  = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr   = 1;
    for (unsigned int i = 0; i < mesh.textures.size(); i++) {
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
	glClearColor(color.red(), color.green(), color.blue(), color.alpha());
}

void Eng::GLRenderer::Clear(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Eng::GLRenderer::AddStaticPointLights(const std::vector<PointLight>& lights)
{
	_staticPointLights = lights;
}

void Eng::GLRenderer::AddStaticPointLights(Eng::PointLight light)
{
	_staticPointLights.push_back(light);
}
