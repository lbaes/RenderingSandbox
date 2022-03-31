#include "Engine/Core/Input.h"
#include "Engine/Core/Window.h"

namespace Eng {

	Input::Input()
	{
		logger = Logger::GetLogger();
	}

	bool Input::IsKeyDown(Keys key) const
	{
		return key_pressed[KeyToCode(key)] == KeyState::PRESSED || key_pressed[KeyToCode(key)] == KeyState::REPEAT;
	}

	bool Input::IsKeyUp(Keys key) const
	{
		return !IsKeyDown(key);
	}

	bool Input::IsMousePressed(MouseButton btn) const
	{
		return mouse_button_pressed[KeyToCode(btn)] == KeyState::PRESSED;
	}

	bool Input::IsMouseReleased(MouseButton btn) const
	{
		return !IsMousePressed(btn);
	}

	void Input::UpdateKeyState(Keys key, KeyState state) {
		auto key_code = KeyToCode(key);
		if (key_code == -1) return;
		key_pressed[key_code] = state;
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

	bool Input::KeyDown(Keys key)
	{
		bool wasPressed = prev_key_pressed[KeyToCode(key)] == KeyState::PRESSED || prev_key_pressed[KeyToCode(key)] == KeyState::REPEAT;
		prev_key_pressed[KeyToCode(key)] = key_pressed[KeyToCode(key)];
		return !wasPressed && IsKeyDown(key);
	}

	bool Input::MousePressed(MouseButton btn)
	{
		bool wasPressed = prev_mouse_button_pressed[KeyToCode(btn)] == KeyState::PRESSED || prev_mouse_button_pressed[KeyToCode(btn)] == KeyState::REPEAT;
		prev_mouse_button_pressed[KeyToCode(btn)] = mouse_button_pressed[KeyToCode(btn)];
		return !wasPressed && IsMousePressed(btn);
	}

	void Input::CopyStates()
	{

	}

}