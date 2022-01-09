#pragma once

#include <string>
#include <Engine/Events/EventBase.h>
#include "Engine/Events/EventDispatcher.h"
#include "Engine/Core/Input.h"

namespace Eng
{
	struct WindowData {
		int width;
		int height;
	};

	class Window
	{
	public:
		Window(EventDispatcher* dispatcher);
		virtual ~Window() = default;
		virtual Window* CreateWindow(const std::string& windowTitle, int width, int height) = 0;
		
		// Implement Module
		virtual void Start();
		virtual void Update();
		virtual void Shutdown();

		// Window Methods
		virtual void CloseWindow() = 0;
		virtual int GetWidth() const = 0;
		virtual int GetHeight() const = 0;
	protected:
		EventDispatcher* dispatcher;
		std::unique_ptr<Input> input;
	private:
		WindowData windowData;
	};
}