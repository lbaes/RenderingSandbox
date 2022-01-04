#include "Engine/Core/Application.h"
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <stdexcept>

#ifdef ENG_WINDOWS
#include "Engine/Platform/GLFW/GlfwWindow.h"
#endif

namespace Eng
{
	Application::Application() : isRunning{ false }
	{
		app = this;
		window = GetWindow();
		window->CreateWindow("Application", 800, 400);
		dispatcher->add_listener(this, &Application::OnQuitEvent);
	};

	Application* Application::app = nullptr;
	EventDispatcher* Application::dispatcher = nullptr;

	Application::~Application() {
		dispatcher->remove_listener(this, &Application::OnQuitEvent);
	}

	int Application::Start()
	{
		isRunning = true;
		while (isRunning)
		{
			window->Update();
		}
		return 0;
	}

	Application* GetApplication()
	{
		return Application::app;
	}

	void Application::OnQuitEvent(const QuitEvent& event)
	{
		isRunning = false;
	}

#ifdef ENG_WINDOWS
	Window* GetWindow() {
		static EventDispatcher dispatcher;
		Application::dispatcher = &dispatcher;
		static GlfwWindow window(Application::dispatcher);
		return &window;
	}
#endif
}