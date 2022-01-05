#pragma once

#include <string>
#include <Engine/Events/EventBase.h>
#include "Engine/Events/EventDispatcher.h"
#include "Engine/Core/Input.h"

namespace Eng
{
	struct WindowData {
		int width;
		int height;
	};

	class Window
	{
	public:
		Window(EventDispatcher* dispatcher);
		virtual ~Window() = default;
		virtual Window* CreateWindow(const std::string& windowTitle, int width, int height) = 0;
		virtual void Update() = 0;
		virtual void Close() = 0;

		virtual int GetWidth() const = 0;
		virtual int GetHeight() const = 0;
	protected:
		EventDispatcher* dispatcher;
		std::unique_ptr<Input> input;
	private:
		WindowData windowData;
	};
}

/*
		void DispatchEvent(const EventBase& event) const {
			dispatcher->dispatch(event);
		}

		template<typename T, typename DerivedEvent>
		void AddListener(void(T::* callbackT)(const DerivedEvent&)) {
			dispatcher->template add_listener(this, callbackT);
		}

		template<typename T, typename DerivedEvent>
		void RemoveListener(void(T::* callbackT)(const DerivedEvent&)) {
			dispatcher->template remove_listener(this, callbackT);
		}
*/