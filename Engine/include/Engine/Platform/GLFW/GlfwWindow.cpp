#include "GlfwWindow.h"
#include "Engine/Core/Logger.h"

namespace Eng
{

	Window* GlfwWindow::CreateWindow(const std::string& windowTitle, int width, int height)
	{
		mWidth = width;
		mHeight = height;
		if (!glfwInit())
		{
			throw std::runtime_error("GLFW failed to initialize");
		}
		windowHandle = glfwCreateWindow(width, height, windowTitle.c_str(), nullptr, nullptr);
		if (!windowHandle)
		{
			glfwTerminate();
			throw std::runtime_error("GLFW failed to create window");
		}
		return this;
	}

	void GlfwWindow::DestroyWindow()
	{
		glfwDestroyWindow(windowHandle);
	}

	void GlfwWindow::OnCreate()
	{
		auto logger = Logger::GetLogger();
		glfwMakeContextCurrent(windowHandle);
		gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		glViewport(0, 0, GetWidth(), GetHeight());
		logger->LogInfo("GLFW Window created with valid OpenGl Context");
	}

	void GlfwWindow::OnUpdate()
	{
		glfwPollEvents();
	}

	int GlfwWindow::GetWidth() const
	{
		return mWidth;
	}

	int GlfwWindow::GetHeight() const
	{
		return mHeight;
	}

	GLFWwindow* GlfwWindow::GetHandle() const
	{
		return windowHandle;
	}

	void GlfwWindow::OnClose(GLFWwindow* handle)
	{
		glfwDestroyWindow(handle);
	}

}