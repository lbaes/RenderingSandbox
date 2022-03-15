#pragma once

#include <memory>
#include "Core.h"
#include "Window.h"
#include "Engine/Events/EventDispatcher.h"
#include "Engine/Events/Events.h"
#include "Engine/Core/Input.h"
#include "Engine/Resources/ResourceManager.h"
#include "Engine/LLRenderer/RenderDevice.h"

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

	private:
		friend EG_API Application* GetApplication();
		friend Window* GetWindow();
		friend Input* GetInput();

		Window* window;
		Input* input;
		std::unique_ptr<ResourceManager> resourceManager;
		std::unique_ptr<RenderDevice> renderDevice;
		bool isRunning;

	};
}