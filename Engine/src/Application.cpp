#include "Engine/Core/Application.h"
#include "Engine/Platform/Timer.h"
#include <stdexcept>
#include <memory>

#if defined(ENG_WINDOWS) || defined(ENG_LINUX)
#include "Engine/Platform/GLFW/GlfwWindow.h"
#include "Engine/Platform/GLFW/GlfwInput.h"
#endif

namespace Eng
{
	Application::Application() : window{ nullptr }, input{nullptr}, isRunning{ false }
	{
		app = this;
		window = GetWindow();
		window->CreateWindow("Application", 800, 400);
		input = GetInput();
		resourceManager = std::make_unique<ResourceManager>();
		renderDevice = std::make_unique<RenderDevice>();
	};

	Application* Application::app = nullptr;

	Application::~Application() {
	}

	int Application::Start()
	{
		Timer timer;
		Ticks frametime = 0.1;
		std::stringstream timeText;
		isRunning = true;

		auto handle = resourceManager->LoadTexture("resources/bricks.jpeg");
		renderDevice->InitRenderDevice();
		renderDevice->CreateTexture2D(*handle.resource);

		while (isRunning)
		{
			timer.Start();
			timeText.str("");
			timeText << "fTime: " << frametime << " | FPS: " << 1/(frametime/1000);
			window->SetTitle(timeText.str());
			isRunning = window->IsOpen();
			input->Update();
			if (input->IsKeyDown(Keys::ESCAPE)) {
				isRunning = false;
			}
			window->Update();
			frametime = timer.GetTime();
			timer.Stop();
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