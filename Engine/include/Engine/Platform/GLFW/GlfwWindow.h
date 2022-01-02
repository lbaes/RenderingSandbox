#pragma once

#include "Engine/Core/Window.h"
#include "Engine/Events/Events.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace Eng
{
	class GlfwWindow : public Window
	{
	public:
		Window* CreateWindow(const std::string& windowTitle, int width, int height) override;

		void DestroyWindow() override;

		int GetWidth() const override;

		int GetHeight() const override;

		GLFWwindow* GetHandle() const;

		void OnCreate() override;
		void OnUpdate() override;

	protected:
		static void OnClose(GLFWwindow* handle);
	private:
		int mWidth = 0;
		int mHeight = 0;
		GLFWwindow* windowHandle;
	};
}


