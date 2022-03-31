#pragma once
#include "Engine/Events/EventDispatcher.h"
#include "Engine/Events/Events.h"
#include "Engine/Core/Keys.h"
#include "Engine/Core/Logger.h"
#include <array>

namespace Eng {
	class Window;
	class Input {
	public:
		Input();
		virtual void Update(){};
		// current update
		bool IsKeyDown(Keys key) const;
		bool IsKeyUp(Keys key) const;
		bool IsMousePressed(MouseButton btn) const;
		bool IsMouseReleased(MouseButton btn) const;

		// changed since last update
		bool KeyDown(Keys key);
		bool MousePressed(MouseButton btn);
	protected:
		void UpdateKeyState(Keys key, KeyState state);
		void UpdateMouseButtonState(MouseButton btn, KeyState state);
		void UpdateMousePosition(double xPos, double yPos);
		void UpdateMouseScroll(double xPos, double yPos);
		void CopyStates();
	private:
		std::array<KeyState, static_cast<int> (Keys::LAST)> key_pressed{KeyState::RELEASED};
		std::array<KeyState, static_cast<int> (MouseButton::LAST)> mouse_button_pressed{ KeyState::RELEASED };
		std::array<KeyState, static_cast<int> (Keys::LAST)> prev_key_pressed{KeyState::RELEASED};
		std::array<KeyState, static_cast<int> (MouseButton::LAST)> prev_mouse_button_pressed{ KeyState::RELEASED };
		double mouseX{}, mouseY{}, prevMouseX{}, prevMouseY{};
		double scrollX{}, scrollY{}, prevScrollX{}, prevScrollY{};
		Logger* logger = nullptr;
	};
}