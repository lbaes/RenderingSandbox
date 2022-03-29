#include "Engine/Platform/GLFW/GlfwInput.h"

namespace Eng {

	GlfwInput* GlfwInput::input_ptr = nullptr;

	GlfwInput::GlfwInput(const GlfwWindow& window)
		: Input()
	{
		GlfwInput::input_ptr = this;
		auto handle = window.GetHandle();
		glfwSetKeyCallback(handle, &GlfwInput::key_callback);
		glfwSetCursorPosCallback(handle, &GlfwInput::cursor_position_callback);
		glfwSetMouseButtonCallback(handle, &GlfwInput::mouse_button_callback);
		glfwSetScrollCallback(handle, &GlfwInput::scroll_callback);
	}

	void GlfwInput::Update()
	{
		// Triggers all registered callbacks;
		glfwPollEvents();
	}

	void GlfwInput::cursor_position_callback(GLFWwindow*, double xpos, double ypos)
	{
		input_ptr->UpdateMousePosition(xpos, ypos);
	}

	void GlfwInput::mouse_button_callback(GLFWwindow*, int button, int action, [[maybe_unused]] int mods)
	{
		auto btn = MouseButtonCodeToMouseButton(button);
		auto kState = CodeToKeyState(action);
		input_ptr->UpdateMouseButtonState(btn, kState);
	}

	void GlfwInput::scroll_callback(GLFWwindow*, double xoffset, double yoffset)
	{
		input_ptr->UpdateMouseScroll(xoffset, yoffset);
	}

	void GlfwInput::key_callback(GLFWwindow*, int key, [[maybe_unused]]int scancode, int action, [[maybe_unused]]int mods)
	{
		auto key_ = KeyCodeToKey(key);
		auto keyState = CodeToKeyState(action);
		input_ptr->UpdateKeyState(key_, keyState);
	}

	constexpr KeyState GlfwInput::CodeToKeyState(int state)
	{
		return static_cast<KeyState>(state);
	}

	constexpr Modifiers GlfwInput::CodeToModifiers(int modifiers)
	{
		return static_cast<Modifiers>(modifiers);
	}

	constexpr MouseButton GlfwInput::MouseButtonCodeToMouseButton(int mouseButton)
	{
		return static_cast<MouseButton>(mouseButton);
	}

	constexpr Keys GlfwInput::KeyCodeToKey(int key)
	{
		return static_cast<Keys>(key);
	}
}