#pragma once
#include <iostream>
#include "Engine/Engine.h"
#include "Engine/Platform/Timer.h"
#include "Engine/Resources/Shader.h"
#include "Engine/Resources/Model.h"

using namespace Eng;

class Demo : public Eng::Application {
public:
	// GPU handles
	GPUShaderHandle model_shader_handle;
	GPUShaderHandle line_shader_handle;
	GPUModelHandle model_handle;
    GPULineHandle line_handle;
    GPULineHandle line_handle2;

	// Timers
	Timer simulation_timer;
	double current;

	// Model position
	Vec3 model_pos = Vec3{0.0, 0.0, 0.0};
	Vec3 model_scale = Vec3{1.0, 1.0, 1.0};
	Vec3 model_rotation = Vec3{1.0f, 0.0f, 0.0f};
	float rotation_angle = 90.0f;
	Transform t;

	// Camera settings
	Camera camera;
	float camZ = 0.0f;
	float camX = 0.0f;
	float camY = 1.0f;
	float aspectRatio;

	void OnStart() override {
		std::cout << "Demo Start\n";
		aspectRatio = (float) window->GetPixelWidth() / (float) window->GetPixelHeight();

		// Load shaders
		Shader vShaderModel = Eng::Shader::LoadFromDisk("resources/vModel.glsl");
		Shader fShaderModel = Eng::Shader::LoadFromDisk("resources/fModel.glsl");

        Shader vShaderLine = Shader::LoadFromDisk("resources/vLine.glsl");
        Shader fShaderLine = Shader::LoadFromDisk("resources/fLine.glsl");

        model_shader_handle = renderDevice->CreateShaderProgram(vShaderModel, fShaderModel);
        line_shader_handle = renderDevice->CreateShaderProgram(vShaderLine, fShaderLine);

        // Load model
		Model model;
		model.LoadFromFile("resources/backpack/backpack.gltf");
		model_handle = renderDevice->CreateModel(model);

        // Grid
        Line line;
        line.p1 = {-10000.0f, -0.0f, 0.0f};
        line.p2 = {10000.0f, 0.0f, 0.0f};

        Line line2;
        line2.p1 = {0.0f, 0.0f, -10000.0f};
        line2.p2 = {0.0f, 0.0f, 10000.0f};

        line_handle = renderDevice->CreateLine(line);
        line_handle2 = renderDevice->CreateLine(line2);

        // Lights
        Vec3 light_pos = {5.0f, 10.0f, 5.0f};
        model_shader_handle.use();
        model_shader_handle.uniform_set("light.position", light_pos);
        model_shader_handle.uniform_set("light.diffuse", Vec3{1.0, 1.0, 1.0});
        model_shader_handle.uniform_set("light.ambient", Vec3{0.1, 0.1, 0.1});
        model_shader_handle.uniform_set("light.specular", Vec3{0.0, 0.0, 0.0});

		renderer->SetCamera(camera);

		// Start timer;
		simulation_timer.Start();
	}

	void OnUpdate() override {
		using namespace Eng;
		current = simulation_timer.GetTime();
		aspectRatio = (float) window->GetPixelWidth() / (float) window->GetPixelHeight();

		t = {model_pos, model_scale, model_rotation, rotation_angle};

		camZ = cos(current / 1000) * 8.0;
		camX = sin(current / 1000) * 8.0;

		if (input->IsKeyDown(Keys::DOWN))
			camY--;
		else if (input->IsKeyDown(Keys::UP))
			camY++;

		Vec3 cameraUp = Vec3{0.0, 1.0, 0.0};
		Vec3 cameraPos = Vec3{camX, camY, camZ};

		// Setup Camera
		camera.SetCameraPos({cameraPos, model_pos, cameraUp, -90.0f, 0.0f});
		camera.SetProjection(45, aspectRatio);
		renderer->SetCamera(camera);
	}

	void Draw() override
	{
		// Render stuff
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw model
        renderer->SetShader(model_shader_handle);
		renderer->RenderModel(model_handle, t);

        // draw lines
        renderer->SetShader(line_shader_handle);
        renderer->RenderLine(line_handle);
        renderer->RenderLine(line_handle2);
	}
};


