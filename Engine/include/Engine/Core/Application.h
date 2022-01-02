#pragma once

#include "Core.h"
#include "Window.h"

namespace Eng
{
	class EG_API Application;

	EG_API Application* GetApplication();

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
		Window* window;
		bool isRunning;
	};
}