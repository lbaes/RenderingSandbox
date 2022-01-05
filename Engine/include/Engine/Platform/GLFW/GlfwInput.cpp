#include "Engine/Platform/GLFW/GlfwInput.h"
#include "Engine/Core/Application.h"

namespace Eng {

	GlfwInput* GlfwInput::input_ptr = nullptr;

	GlfwInput::GlfwInput(EventDispatcher* dispatcher, GlfwWindow* window)
		: Input(dispatcher)
	{
		GlfwInput::input_ptr = this;
		auto handle = window->GetHandle();
		glfwSetKeyCallback(handle, &GlfwInput::key_callback);
		glfwSetCursorPosCallback(handle, &GlfwInput::cursor_position_callback);
		glfwSetMouseButtonCallback(handle, &GlfwInput::mouse_button_callback);
		glfwSetScrollCallback(handle, &GlfwInput::scroll_callback);
	}

	void GlfwInput::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
	{
		input_ptr->UpdateMousePosition(xpos, ypos);
	}

	void GlfwInput::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
	{
		auto btn = MouseButtonCodeToMouseButton(button);
		auto kState = CodeToKeyState(action);
		input_ptr->UpdateMouseButtonState(btn, kState);
	}

	void GlfwInput::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		input_ptr->UpdateMouseScroll(xoffset, yoffset);
	}

	void GlfwInput::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		auto key_ = KeyCodeToKey(key);
		auto keyState = CodeToKeyState(action);
		input_ptr->UpdateKeyState(key_, keyState);
	}

	inline constexpr KeyState GlfwInput::CodeToKeyState(int state)
	{
		return static_cast<KeyState>(state);
	}

	inline constexpr Modifiers GlfwInput::CodeToModifiers(int modifiers)
	{
		return static_cast<Modifiers>(modifiers);
	}

	inline constexpr MouseButton GlfwInput::MouseButtonCodeToMouseButton(int mouseButton)
	{
		return static_cast<MouseButton>(mouseButton);
	}

	inline constexpr Keys GlfwInput::KeyCodeToKey(int key)
	{
		return static_cast<Keys>(key);
	}
}