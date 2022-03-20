#include "Engine/Core/Application.h"
#include "Engine/Core/Logger.h"
#include "Engine/Platform/Timer.h"
#include "Engine/Resources/Model.h"
#include "Engine/Resources/Shader.h"
#include <memory>

#if defined(ENG_WINDOWS) || defined(ENG_LINUX)
#include "Engine/Platform/GLFW/GlfwWindow.h"
#include "Engine/Platform/GLFW/GlfwInput.h"
#endif

namespace Eng
{
	Application::Application() : window{ nullptr }, input{ nullptr }, isRunning{ false }
	{
		app = this;
		window = GetWindow();
		window->CreateWindow("Application", 800, 400);
		input = GetInput();
		renderDevice = std::make_unique<RenderDevice>();
		renderer = std::make_unique<Renderer>();
        logger = Logger::GetLogger();
	};

	Application* Application::app = nullptr;

	Application::~Application() {
	}

	int Application::Start()
	{
		renderDevice->InitRenderDevice();
		isRunning = true;
		OnStart();

        glClearColor(52.0/255.0, 186.0/255.0, 235.0/255.0, 255.0/255.0);

        // load shaders
        Shader vShader = Shader::LoadFromDisk("resources/vModel.glsl");
        Shader fShader = Shader::LoadFromDisk("resources/fModel.glsl");
        auto shader_handle = renderDevice->CreateShaderProgram(vShader, fShader);

        // Load model
        Model model;
        model.LoadFromFile("resources/monkey/monkey.obj");
        auto model_handle = renderDevice->CreateModel(model);

        // Setup Camera
        Camera camera;
        camera.SetCameraPos({{0.0, 0.0, 3.0},
                             {0.0, 0.0, 0.0},
                             {0.0, 1.0, 0.0}, -90.0f, 0.0f});
        float aspectRatio = static_cast<float>(window->GetWidth())/static_cast<float>(window->GetHeight());
        camera.SetProjection(90, aspectRatio);

        // Setup transform
        Transform t{{0.0, 0.0, 0.0},
                    {10.0, 10.0, 10.0},
                    {1.0f, 0.0f, 0.0f}, 0.0f};

        // Setup renderer
        renderer->SetShader(shader_handle);
        renderer->SetCamera(camera);

		const double ms_per_update = 50.0;
		Timer simulation_timer;
		simulation_timer.Start();
		double previous = simulation_timer.GetTime();
		double lag = 0.0;
		while (window->IsOpen())
		{
			double current = simulation_timer.GetTime();
			double elapsed = current - previous;
			previous = current;
			lag += elapsed;
			input->Update();
//			while (lag >= ms_per_update) {
//
//			}
            OnUpdate();
            window->Update();
            lag -= ms_per_update;
			// Render stuff
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            renderer->RenderModel(model_handle, t);
		}
		return 0;
	}

	Application* GetApplication()
	{
		return Application::app;
	}

#ifdef ENG_WINDOWS
	Window* GetWindow() {
		static GlfwWindow window;
		return &window;
	}

	Input* GetInput() {
		auto window = dynamic_cast<GlfwWindow*>(GetWindow());
		static GlfwInput input(*window);
		return &input;
	}
#endif
}
