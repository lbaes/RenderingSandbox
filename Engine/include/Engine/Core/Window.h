#pragma once

#include <string>
#include <Engine/Events/EventBase.h>

namespace Eng
{
	struct WindowData {
		int width;
		int height;
	};

	class Window
	{
	public:
		Window();
		virtual ~Window() = default;
		virtual Window* CreateWindow(const std::string& windowTitle, int width, int height) = 0;
		virtual void Update(){};
		virtual void CloseWindow() = 0;
		virtual int GetWidth() const = 0;
		virtual int GetHeight() const = 0;
		virtual void SetTitle(const std::string& title) const = 0;
		bool IsOpen();
	protected:
		void SetWindowShouldClose();
	private:
		WindowData windowData;
		bool isOpen = true;
	};
}