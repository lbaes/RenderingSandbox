#include "Engine/Core/Application.h"
#include "Engine/Core/Logger.h"
#include "Engine/Platform/Timer.h"
#include "Engine/Resources/Model.h"
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

		// load stuff
        Model model;
        model.LoadFromFile("resources/monkey/monkey.obj");
        auto model_handle = renderDevice->CreateModel(model);

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
			while (lag >= ms_per_update) {
				OnUpdate();
				window->Update();
				lag -= ms_per_update;
			}
			// Render stuff

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
