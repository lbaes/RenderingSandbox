#include "Engine/Core/Input.h"
#include "Engine/Core/Window.h"

namespace Eng {

	Input::Input()
	{
		logger = Logger::GetLogger();
	}

	bool Input::IsKeyDown(Keys key)
	{
		return key_pressed[KeyToCode(key)] == KeyState::PRESSED;
	}

	bool Input::IsKeyUp(Keys key)
	{
		return !IsKeyDown(key);
	}

	bool Input::IsMousePressed(MouseButton btn)
	{
		return mouse_button_pressed[KeyToCode(btn)] == KeyState::PRESSED;
	}

	bool Input::IsMouseReleased(MouseButton btn)
	{
		return !IsMousePressed(btn);
	}

	void Input::UpdateKeyState(Keys key, KeyState state) {
		key_pressed[KeyToCode(key)] = state;
		KeyEvent e{};
		e.isHandled = false;
		e.key = key;
		e.mods = Modifiers::NONE;
		e.state = state;
		if (state == KeyState::PRESSED) {
			logger->LogTrace("Key press");
		}
		else if (state == KeyState::RELEASED)
		{
			logger->LogTrace("Key release");
		}
		else {
			logger->LogTrace("Key repeat");
		}
	}

	void Input::UpdateMouseButtonState(MouseButton btn, KeyState state) {
		mouse_button_pressed[KeyToCode(btn)] = state;
		MouseButtonEvent e;
		e.isHandled = false;
		e.button = btn;
		e.state = state;
		e.mods = Modifiers::NONE;
	}

	void Input::UpdateMousePosition(double xPos, double yPos)
	{
		mouseX = xPos;
		mouseY = yPos;
		MouseMovedEvent e;
		e.isHandled = false;
		e.x_pos = xPos;
		e.y_pos = yPos;
	}

	void Input::UpdateMouseScroll(double xPos, double yPos)
	{
		scrollX = xPos;
		scrollY = yPos;
		MouseScrollEvent e;
		e.isHandled = false;
		e.x_offset = xPos;
		e.y_offset = yPos;
	}

}