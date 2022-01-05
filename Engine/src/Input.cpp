#include "Engine/Core/Input.h"

namespace Eng {

	Input::Input(EventDispatcher* dispatcher) : dispatcher{ dispatcher }
	{
		assert(dispatcher != nullptr);
		logger = Logger::GetLogger();
	}

	bool Input::IsKeyDown(Keys key)
	{
		return key_pressed.at(key) == KeyState::PRESSED;
	}

	bool Input::IsKeyUp(Keys key)
	{
		return !IsKeyDown(key);
	}

	bool Input::IsMousePressed(MouseButton btn)
	{
		return mouse_button_pressed.at(btn) == KeyState::PRESSED;
	}

	bool Input::IsMouseReleased(MouseButton btn)
	{
		return !IsMousePressed(btn);
	}

	void Input::UpdateKeyState(Keys key, KeyState state) {
		key_pressed[key] = state;
		KeyEvent e{};
		e.isHandled = false;
		e.key = key;
		e.mods = Modifiers::NONE;
		e.state = state;
		dispatcher->dispatch(e);
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
		mouse_button_pressed[btn] = state;
		MouseButtonEvent e;
		e.isHandled = false;
		e.button = btn;
		e.state = state;
		e.mods = Modifiers::NONE;
		dispatcher->dispatch(e);
	}

	void Input::UpdateMousePosition(double xPos, double yPos)
	{
		mouseX = xPos;
		mouseY = yPos;
		MouseMovedEvent e;
		e.isHandled = false;
		e.x_pos = xPos;
		e.y_pos = yPos;
		dispatcher->dispatch(e);
	}

	void Input::UpdateMouseScroll(double xPos, double yPos)
	{
		scrollX = xPos;
		scrollY = yPos;
		MouseScrollEvent e;
		e.isHandled = false;
		e.x_offset = xPos;
		e.y_offset = yPos;
		dispatcher->dispatch(e);
	}

}