#pragma once

#include "Core.h"
#include "Window.h"
#include "Engine/Events/EventDispatcher.h"
#include "Engine/Events/Events.h"

namespace Eng
{
	class EG_API Application;

	EG_API Application* GetApplication();
	Window* GetWindow();

	class EG_API Application
	{
	public:
		Application();
		virtual ~Application();
		int Start();

	protected:
		static Application* app;

	private:
		friend EG_API Application* GetApplication();
		friend Window* GetWindow();
		static EventDispatcher* dispatcher;
		Window* window;
		bool isRunning;

		void OnQuitEvent(const QuitEvent& event);

	};
}