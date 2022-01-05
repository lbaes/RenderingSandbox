#pragma once
#include "Engine/Events/EventDispatcher.h"
#include "Engine/Events/Events.h"
#include "Engine/Core/Keys.h"
#include "Engine/Core/Logger.h"

namespace Eng {
	class Input {
	public:
		Input(EventDispatcher* dispatcher);
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
		std::unordered_map<Keys, KeyState> key_pressed;
		std::unordered_map<MouseButton, KeyState> mouse_button_pressed;
		double mouseX{}, mouseY{};
		double scrollX{}, scrollY{};
		EventDispatcher* dispatcher = nullptr;
		Logger* logger = nullptr;
	};
}