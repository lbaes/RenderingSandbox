#pragma once
#include "Engine/Core/Input.h"
#include "Engine/Platform/GLFW/GlfwWindow.h"
#include "GLFW/glfw3.h"

namespace Eng {
	class GlfwInput : public Input {
	public:
		GlfwInput(const GlfwWindow& window);
		virtual void Update() override;
	private:
		static GlfwInput* input_ptr;
		static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
		static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
		static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static constexpr KeyState CodeToKeyState(int state);
		static constexpr Modifiers CodeToModifiers(int modifiers);
		static constexpr MouseButton MouseButtonCodeToMouseButton(int mouseButton);
		static constexpr Keys KeyCodeToKey(int key);

	};
}