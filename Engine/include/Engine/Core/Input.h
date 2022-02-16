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
		bool IsKeyDown(Keys key);
		bool IsKeyUp(Keys key);
		bool IsMousePressed(MouseButton btn);
		bool IsMouseReleased(MouseButton btn);
	protected:
		void UpdateKeyState(Keys key, KeyState state);
		void UpdateMouseButtonState(MouseButton btn, KeyState state);
		void UpdateMousePosition(double xPos, double yPos);
		void UpdateMouseScroll(double xPos, double yPos);
	private:
		std::array<KeyState, static_cast<int> (Keys::LAST)> key_pressed{KeyState::RELEASED};
		std::array<KeyState, static_cast<int> (MouseButton::LAST)> mouse_button_pressed{ KeyState::RELEASED };
		double mouseX{}, mouseY{};
		double scrollX{}, scrollY{};
		Logger* logger = nullptr;
	};
}