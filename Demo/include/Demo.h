#pragma once
#include <iostream>
#include "Engine/Engine.h"
#include "Engine/Platform/Timer.h"
#include "Engine/Resources/Shader.h"
#include "Engine/Resources/Model.h"
#include "Engine/LLRenderer/ArcBallCamera.h"


using namespace Eng;

class Demo : public Eng::Application {
public:
	// GPU handles
	GPUShaderHandle model_shader_handle, line_shader_handle, simple_model_shader_handle;
	GPUModelHandle backpack_handle, container_handle;
    GPULineHandle lineX_handle, lineY_handle, lineZ_handle;

	// Timers
	Timer simulation_timer;
	double current;

	// Model position
	Vec3 model_pos = Vec3{0.0, 0.0, 0.0};
	Vec3 model_scale = Vec3{1.0, 1.0, 1.0};
	Vec3 model_rotation = Vec3{1.0f, 0.0f, 0.0f};
	float rotation_angle = 90.0f;
	Transform t, t2;

	// Camera settings
	ArcBallCamera camera;
	float camZ = 5.0f;
	float camX = 0.0f;
	float camY = 1.0f;
	float angleX = 0.0f;
	float angleY = 0.0f;
	float last_angleX = 0.0f;
	float last_angleY = 0.0f;
	float last_mouseX = 0.0f;
	float last_mouseY = 0.0f;
	float aspectRatio = 1.0f;

	// Grid settings
	bool drawGrid = false;

	void OnStart() override {
		std::cout << "Demo Start\n";
		aspectRatio = (float) window->GetPixelWidth() / (float) window->GetPixelHeight();

		// Load shaders
		Shader vShaderModel, fShaderModel, vShaderLine, fShaderLine;
		unsigned int effects = ShaderEffects::TEXTURES | ShaderEffects::AMBIENT_LIGHT | ShaderEffects::NORMAL_MAP | ShaderEffects::SPECULAR_MAP;
		unsigned int effects_simple = ShaderEffects::TEXTURES | ShaderEffects::AMBIENT_LIGHT | ShaderEffects::SPECULAR_MAP;

		vShaderModel.LoadFromDisk("resources/vUberShader.glsl");
		fShaderModel.LoadFromDisk("resources/fUberShader.glsl");

        vShaderLine.LoadFromDisk("resources/vLine.glsl");
        fShaderLine.LoadFromDisk("resources/fLine.glsl");

		vShaderModel.ConfigureEffects(effects);
		fShaderModel.ConfigureEffects(effects);
        model_shader_handle = renderDevice->CreateShaderProgram(vShaderModel, fShaderModel);

		fShaderModel.ConfigureEffects(effects_simple);
        simple_model_shader_handle = renderDevice->CreateShaderProgram(vShaderModel, fShaderModel);
        line_shader_handle = renderDevice->CreateShaderProgram(vShaderLine, fShaderLine);

        // Load model
		Model backpackModel, containerModel;
		backpackModel.LoadFromFile("resources/backpack/backpack.gltf");
		containerModel.LoadFromFile("resources/cube/container.gltf");
		backpack_handle = renderDevice->CreateModel(backpackModel);
		container_handle = renderDevice->CreateModel(containerModel);

        // Grid
        Line lineX;
		lineX.p1 = { -10000.0f, -0.0f, 0.0f};
		lineX.p2 = { 10000.0f, 0.0f, 0.0f};

        Line lineZ;
		lineZ.p1 = { 0.0f, 0.0f, -10000.0f};
		lineZ.p2 = { 0.0f, 0.0f, 10000.0f};

		Line lineY;
		lineY.p1 = { 0.0f, -10000.0f, 0.0f};
		lineY.p2 = { 0.0f, 10000.0f, 0.0f};

		lineX_handle = renderDevice->CreateLine(lineX);
		lineZ_handle = renderDevice->CreateLine(lineZ);
		lineY_handle = renderDevice->CreateLine(lineY);

        // Lights
        Vec3 light_pos = {0.0f, 3.0f, 3.0f};
        model_shader_handle.use();
        model_shader_handle.uniform_set("light.position", light_pos);
        model_shader_handle.uniform_set("light.diffuse", Vec3{0.5, 0.5, 0.5});
        model_shader_handle.uniform_set("light.ambient", Vec3{0.01});
        model_shader_handle.uniform_set("light.specular", Vec3{1.0, 1.0, 1.0});

		simple_model_shader_handle.use();
		simple_model_shader_handle.uniform_set("light.position", light_pos);
		simple_model_shader_handle.uniform_set("light.diffuse", Vec3{0.5, 0.5, 0.5});
		simple_model_shader_handle.uniform_set("light.ambient", Vec3{0.01});
		simple_model_shader_handle.uniform_set("light.specular", Vec3{1.0, 1.0, 1.0});

		// Renderer config
		renderer->SetCamera(camera);
		renderer->SetClearColor(Color4{0.0f, 0.0f, 0.0f, 1.0f});

		// Camera
		Vec3 cameraUp = Vec3{0.0, 1.0, 0.0};
		Vec3 cameraPos = Vec3{camX, camY, camZ};
		camera.UpdateCamera(cameraPos, model_pos, cameraUp);
		camera.UpdateProjection(45, aspectRatio);

		// Start timer;
		simulation_timer.Start();
	}

	void OnUpdate() override {
		using namespace Eng;
		current = simulation_timer.GetTime();
		auto width = (float) window->GetPixelWidth();
		auto height = (float) window->GetPixelHeight();
		if (width != 0 && height != 0)
			aspectRatio = (float) window->GetPixelWidth() / (float) window->GetPixelHeight();

		t = {model_pos, model_scale, model_rotation, rotation_angle};
		t2 = {model_pos + Vec3{0.0f, -7.0f, 0.0f}, Vec3{1.0f, 1.0f, 1.0f}, model_rotation, 0.0f};

		if (input->KeyDown(Keys::SPACE)){
			drawGrid = !drawGrid;
		}

		if (input->KeyDown(Keys::F)){
			camera.UpdateCamera(Vec3{camX, -6.0f, 3.0}, model_pos + Vec3{0.0f, -7.0f, 0.0f}, Vec3{0.0, 1.0, 0.0});
		}

		if (input->KeyDown(Keys::G)){
			camera.UpdateCamera(Vec3{camX, camY, camZ}, model_pos, Vec3{0.0, 1.0, 0.0});
		}

		last_angleY = angleY;
		last_angleX = angleX;

		if (input->IsKeyDown(Keys::DOWN)){
			angleY -= 1;
		}
		else if (input->IsKeyDown(Keys::UP)){
			angleY += 1;
		}

		if (input->IsKeyDown(Keys::RIGHT)){
			angleX -= 1;
		}
		else if (input->IsKeyDown(Keys::LEFT)){
			angleX += 1;
		}

		// Setup Camera
		camera.UpdateCameraPosByAngle(last_angleX - angleX, last_angleY - angleY);
		camera.UpdateProjection(45, aspectRatio);
		renderer->SetCamera(camera);
	}

	void Draw() override
	{
		// Render stuff
		renderer->Clear();

        // draw model
        renderer->SetShader(model_shader_handle);
		renderer->RenderModel(backpack_handle, t);
		renderer->SetShader(simple_model_shader_handle);
		renderer->RenderModel(container_handle, t2);

        // draw line
		if (drawGrid)
		{
			Color4 lineZ_color{ 0.0, 0.0, 1.0, 1.0 };
			Color4 lineX_color{ 1.0, 0.0, 0.0, 1.0 };
			Color4 lineY_color{ 0.0, 1.0, 0.0, 1.0 };
			renderer->SetShader(line_shader_handle);
			renderer->RenderLine(lineX_handle, lineX_color);
			renderer->RenderLine(lineZ_handle, lineZ_color);
			renderer->RenderLine(lineY_handle, lineY_color);
		}
	}
};


