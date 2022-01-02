#pragma once
#include "EventBase.h"
#include "Engine/Core/Keys.h"

namespace Eng
{
	struct KeyEvent : public EventBase
	{
		Keys key;
		KeyState state;
		Modifiers mods;
	};

	struct MouseMovedEvent : public EventBase
	{
		double x_pos;
		double y_pos;
	};

	struct MouseButtonEvent : public EventBase
	{
		MouseButton button;
		KeyState state;
		Modifiers mods;
	};

	struct MouseScrollEvent : public EventBase
	{
		double x_offset;
		double y_offset;
	};

	struct UpdateEvent : public EventBase{
		float deltaTime = 0.0f;
	};

	class QuitEvent : public EventBase
	{
	};
}