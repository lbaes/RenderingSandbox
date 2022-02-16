#include "Engine/Core/Window.h"
#include "Engine/Platform/GLFW/GlfwInput.h"

namespace Eng {

	Window::Window() : windowData{}
	{
	}

	void Window::SetWindowShouldClose() {
		isOpen = false;
	}

	bool Window::IsOpen() {
		return isOpen;
	}
}