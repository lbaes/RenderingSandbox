#pragma once
#include <iostream>
#include "Engine/Engine.h"
#include "Engine/Platform/Timer.h"
#include "Engine/Resources/Shader.h"
#include "Engine/Resources/Model.h"
#include "Engine/LLRenderer/ArcBallCamera.h"
#include "Engine/LLRenderer/FreeCamera.hpp"
#include "Engine/LLRenderer/PointLight.h"

using namespace Eng;

class Demo : public Eng::Application {
public:
	// GPU handles
	GPUShaderHandle model_shader_handle, line_shader_handle, simple_model_shader_handle, current_shader;
	GPUModelHandle sponza_handle, container_handle, backpack_handle;
    GPULineHandle lineX_handle, lineY_handle, lineZ_handle;

	// Timers
	Timer simulation_timer;
	double current;

	// Model position
	Vec3 model_pos = Vec3{0.0, 0.0, 0.0};
	Vec3 model_scale = Vec3{1.0, 1.0, 1.0};
	Vec3 model_rotation = Vec3{0.0f, 0.0f, 0.0f};

    Vec3 box_pos = Vec3{180.0f, 300.0f, 0.0f};
    Vec3 box_scale = Vec3{2.0f};
    Vec3 box_rotation = Vec3{0.0f, 0.0f, 0.0f};

    Vec3 backpack_pos = Vec3{250.0f, 300.0f, 0.0f};
    Vec3 backpack_scale = Vec3{3.0f};
    Vec3 backpack_rotation = Vec3{90.0f, 0.0f, 90.0f};

    Transform t, t2, t3;

	// Camera settings
    FreeCamera camera;
	const float camZ = 0.0f;
	const float camX = 200.0f;
	const float camY = 300.0f;
	float angleX = 0.0f;
	float angleY = 0.0f;
	float last_angleX = 0.0f;
	float last_angleY = 0.0f;
	float aspectRatio = 1.0f;
    Vec3 cameraUp = Vec3{0.0, 1.0, 0.0};
    Vec3 cameraPos = Vec3{camX, camY, camZ};

	// Lights
	std::vector<PointLight> lights;

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

        vShaderModel.ConfigureEffects(effects_simple);
		fShaderModel.ConfigureEffects(effects_simple);
        simple_model_shader_handle = renderDevice->CreateShaderProgram(vShaderModel, fShaderModel);
        line_shader_handle = renderDevice->CreateShaderProgram(vShaderLine, fShaderLine);

        // Load model
		Model sponzaModel, boxModel, backpackModel;
		sponzaModel.LoadFromFile("resources/Sponza/Sponza.gltf");
		boxModel.LoadFromFile("resources/cube/container.gltf");
        backpackModel.LoadFromFile("resources/backpack/backpack.gltf");

        sponza_handle = renderDevice->CreateModel(sponzaModel);
		container_handle = renderDevice->CreateModel(boxModel);
        backpack_handle = renderDevice->CreateModel(backpackModel);

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
		PointLight p{};
		p.position = cameraPos;
		p.diffuse = Vec3{0.5, 0.5, 0.5};
		p.ambient = Vec3{0.01};
		p.specular = {1.0, 1.0, 1.0};
		p.constant = 1.0f;
		p.linear = 0.0014f;
		p.quadratic = 0.000007f;
		lights.push_back(p);

		p.position = cameraPos + Vec3{30.0f, 0.0f, 0.0f};
		lights.push_back(p);

		p.position = cameraPos + Vec3{60.0f, 0.0f, 0.0f};
		lights.push_back(p);

		p.position = cameraPos + Vec3{90.0f, 0.0f, 0.0f};
		lights.push_back(p);

		p.position = cameraPos + Vec3{120.0f, 0.0f, 0.0f};
		lights.push_back(p);

		current_shader = model_shader_handle;

		// Renderer config
		renderer->AddStaticPointLights(lights);
		renderer->SetCamera(camera);
		renderer->SetClearColor(Color4{0.0f, 0.0f, 0.0f, 1.0f});

		// Camera
		camera.UpdateCamera(cameraPos, {camX+1, camY, camZ}, cameraUp);
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

		t = {model_pos, model_scale, model_rotation};
		t2 = {box_pos, box_scale, box_rotation};
		t3 = {backpack_pos, backpack_scale, backpack_rotation};

		if (input->KeyDown(Keys::G)){
			drawGrid = !drawGrid;
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

        if (input->IsKeyDown(Keys::SPACE)) {
            camera.UpdateCameraPosByDirection(FreeCamera::Direction::UP, 1);
        }

        if (input->IsKeyDown(Keys::LEFT_SHIFT)) {
            camera.UpdateCameraPosByDirection(FreeCamera::Direction::DOWN, 1);
        }

        if (input->IsKeyDown(Keys::W)) {
            camera.UpdateCameraPosByDirection(FreeCamera::Direction::FRONT, 1);
        }

        if (input->IsKeyDown(Keys::A)) {
            camera.UpdateCameraPosByDirection(FreeCamera::Direction::LEFT, 1);
        }

        if (input->IsKeyDown(Keys::S)) {
            camera.UpdateCameraPosByDirection(FreeCamera::Direction::BACK, 1);
        }

        if (input->IsKeyDown(Keys::D)) {
            camera.UpdateCameraPosByDirection(FreeCamera::Direction::RIGHT, 1);
        }

        // Switch shaders
        if (input->KeyDown(Keys::N)){
            current_shader = model_shader_handle;
        }

        if (input->KeyDown(Keys::M)){
            current_shader = simple_model_shader_handle;
        }


		// Update camera
		camera.UpdateCameraView(last_angleX - angleX, last_angleY - angleY);
        camera.UpdateProjection(45, aspectRatio);
		renderer->SetCamera(camera);
	}

	void Draw() override
	{
		// Render stuff
		renderer->Clear();

        // draw model
        renderer->SetShader(current_shader);
		renderer->RenderModel(sponza_handle, t);
		renderer->RenderModel(backpack_handle, t3);

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


