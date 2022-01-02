#pragma once

#include <string>
#include <Engine/Events/EventBase.h>
#include "Engine/Events/EventDispatcher.h"

namespace Eng
{
	struct WindowData {
		int width;
		int height;
	};

	class Window
	{
	public:
		virtual Window* CreateWindow(const std::string& windowTitle, int width, int height) = 0;

		virtual void DestroyWindow() = 0;

		virtual void OnCreate() = 0;

		virtual void OnUpdate() = 0;

		[[nodiscard]] virtual int GetWidth() const = 0;

		[[nodiscard]] virtual int GetHeight() const = 0;

		void DispatchEvent(const EventBase& event) const;

	protected:
		template<typename T, typename DerivedEvent>
		void AddListener(void(T::*callbackT)(const DerivedEvent&)) {
			dispatcher->template add_listener(this, callbackT);
		}

		template<typename T, typename DerivedEvent>
		void RemoveListener(void(T::*callbackT)(const DerivedEvent&)) {
			dispatcher->template remove_listener(this, callbackT);
		}

	private:
		std::unique_ptr<EventDispatcher> dispatcher;
		WindowData windowData;
	};
}