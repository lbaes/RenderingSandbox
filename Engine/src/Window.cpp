#include "Engine/Core/Window.h"
#include "Engine/Platform/GLFW/GlfwInput.h"

namespace Eng {

	Window::Window(EventDispatcher* dispatcher) : windowData{}, dispatcher{dispatcher}
	{
	}
}