#include "Engine/Core/Window.h"

namespace Eng
{

	void Window::DispatchEvent(const EventBase& event) const
	{
		dispatcher->dispatch(event);
	}

	void Window::OnUpdate() override {

	}

	void Window::OnUpdate() override {

	}

	void Window::DestroyWindow() override {

	}

	void Window::GetWidth() const {
		return windowData.width;
	}

	void Window::GetHeight() const {
		return windowData.height;
	}
}

