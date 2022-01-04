#pragma once
#include "Engine/Events/EventDispatcher.h"
#include "Engine/Core/Keys.h"

namespace Eng {
	class Input {
	public:
		virtual void IsKeyDown(Keys key) = 0;
	private:
		EventDispatcher* dispatcher;
	};
}