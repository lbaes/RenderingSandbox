#pragma once

#include <memory>
#include "Core.h"
#include "Window.h"
#include "Engine/Events/EventDispatcher.h"
#include "Engine/Events/Events.h"
#include "Engine/Core/Input.h"
#include "Engine/Core/Logger.h"
#include "Engine/LLRenderer/RenderDevice.h"
#include "Engine/LLRenderer/Buffers.h"

namespace Eng
{
	class EG_API Application;

	EG_API Application* GetApplication();
	Window* GetWindow();
	Input* GetInput();

	class EG_API Application
	{
	public:
		Application();
		virtual ~Application();
		int Start();

	protected:
		static Application* app;
		virtual void OnStart(){};
		virtual void OnUpdate(){};

	private:
		friend EG_API Application* GetApplication();
		friend Window* GetWindow();
		friend Input* GetInput();

		Window* window;
		Input* input;
		Logger* logger;
		std::unique_ptr<RenderDevice> renderDevice;
		bool isRunning;
	};
}