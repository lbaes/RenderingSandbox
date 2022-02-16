#include "GlfwWindow.h"
#ifdef ENG_WINDOWS
#include "Engine/Platform/GLFW/GlfwWindow.h"
#include "Engine/Platform/GLFW/GlfwInput.h"
#endif

namespace Eng
{

	GlfwWindow::GlfwWindow() : Window()
	{
		logger = Eng::Logger::GetLogger();
		auto handle = GetHandle();
	}

	GlfwWindow::~GlfwWindow()
	{
		glfwDestroyWindow(windowHandle);
		glfwTerminate();
	}

	Window* GlfwWindow::CreateWindow(const std::string& windowTitle, int width, int height)
	{
		windowData.width = width;
		windowData.height = height;
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
		InitGLContext();
		SetupCallbacks();
		logger->LogInfo("GLFW window created with valid OpenGl context");
		return this;
	}

	void GlfwWindow::Update()
	{
		glfwSwapBuffers(windowHandle);
	}

	void GlfwWindow::CloseWindow()
	{
		SetWindowShouldClose();
		glfwSetWindowShouldClose(windowHandle, true);
		glfwTerminate();
	}

	void GlfwWindow::InitGLContext()
	{
		glfwMakeContextCurrent(windowHandle);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			logger->LogError("Failed to load opengl funtion pointers");
		glViewport(0, 0, GetWidth(), GetHeight());
		glfwSwapInterval(1);
	}

	void GlfwWindow::SetupCallbacks()
	{
		glfwSetWindowUserPointer(windowHandle, this);
		glfwSetWindowCloseCallback(windowHandle, &GlfwWindow::CloseCallback);

	}

	void GlfwWindow::CloseCallback(GLFWwindow* windowHandle)
	{
		auto window = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(windowHandle));
		window->CloseWindow();
	}

	void GlfwWindow::SetTitle(const std::string& title) const
	{
		glfwSetWindowTitle(windowHandle, title.c_str());
	}

	int GlfwWindow::GetWidth() const
	{
		return windowData.width;
	}

	int GlfwWindow::GetHeight() const
	{
		return windowData.height;
	}

	GLFWwindow* GlfwWindow::GetHandle() const
	{
		return windowHandle;
	}

}