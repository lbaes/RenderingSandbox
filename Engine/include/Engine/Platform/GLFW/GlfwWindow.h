#pragma once

#include "Engine/Core/Window.h"
#include "Engine/Events/Events.h"
#include "Engine/Core/Logger.h"
#include "glad/glad.h"
#pragma once
#include "GLFW/glfw3.h"

namespace Eng
{
	class GlfwWindow : public Window
	{
	public:
		GlfwWindow(EventDispatcher* dispatcher);
		~GlfwWindow();
		Window* CreateWindow(const std::string& windowTitle, int width, int height) override;
		void Update() override;
		void Close() override;

		int GetWidth() const override;
		int GetHeight() const override;

		GLFWwindow* GetHandle() const;
	private:
		WindowData windowData{};
		GLFWwindow* windowHandle = nullptr;
		Logger* logger = nullptr;
		void InitGLContext();
		void SetupCallbacks();

		static void CloseCallback(GLFWwindow* windowHandle);
	};
}


