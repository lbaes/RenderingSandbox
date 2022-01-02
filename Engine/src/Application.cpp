#include "Engine/Core/Application.h"
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <stdexcept>

namespace Eng
{
	Application::Application() : isRunning{ false }
	{
		app = this;
		window = window->CreateWindow("Application", 800, 400);
	};

	Application* Application::app = nullptr;

	Application::~Application() = default;

	int Application::Start()
	{
		isRunning = true;
		window->OnCreate();
		while (isRunning)
		{
			window->OnUpdate(0.0f);
		}
		return 0;
	}

	Application* GetApplication()
	{
		return Application::app;
	}
}