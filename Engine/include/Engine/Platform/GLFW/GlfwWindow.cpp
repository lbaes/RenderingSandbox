#include "GlfwWindow.h"
#ifdef ENG_WINDOWS
#include "Engine/Platform/GLFW/GlfwWindow.h"
#include "Engine/Platform/GLFW/GlfwInput.h"
#endif

namespace Eng
{
    GlfwWindow* cast_to_window(GLFWwindow* windowHandle);

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
		if (!glfwInit())
		{
			throw std::runtime_error("GLFW failed to initialize");
		}

        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		windowHandle = glfwCreateWindow(width, height, windowTitle.c_str(), nullptr, nullptr);
		if (!windowHandle)
		{
			glfwTerminate();
			throw std::runtime_error("GLFW failed to create window");
		}

        glfwGetWindowSize(windowHandle, &windowData.width, &windowData.height);
        glfwGetFramebufferSize(windowHandle, &windowData.pixel_width, &windowData.pixel_height);
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
			logger->LogError("Failed to load opengl function pointers");
        glViewport(0, 0, GetPixelWidth(), GetPixelHeight());
		glfwSwapInterval(1);
	}

	void GlfwWindow::SetupCallbacks()
	{
		glfwSetWindowUserPointer(windowHandle, this);
		glfwSetWindowCloseCallback(windowHandle, &GlfwWindow::CloseCallback);
        glfwSetFramebufferSizeCallback(windowHandle, &GlfwWindow::FrameBufferResizeCallback);
        glfwSetWindowSizeCallback(windowHandle, &GlfwWindow::WindowResizeCallback);
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

    void GlfwWindow::CloseCallback(GLFWwindow* windowHandle)
    {
        auto window = cast_to_window(windowHandle);
        window->CloseWindow();
    }

    void GlfwWindow::WindowResizeCallback(GLFWwindow* windowHandle, int width, int height) {
        auto window = cast_to_window(windowHandle);
        window->windowData.width = width;
        window->windowData.height = height;
    }

    void GlfwWindow::FrameBufferResizeCallback(GLFWwindow *windowHandle, int width, int height) {
        auto window = cast_to_window(windowHandle);
        window->windowData.pixel_width = width;
        window->windowData.pixel_height = height;
        glViewport(0, 0, width, height);
    }

    int GlfwWindow::GetPixelWidth() const {
        return windowData.pixel_width;
    }

    int GlfwWindow::GetPixelHeight() const {
        return windowData.pixel_height;
    }

    GlfwWindow* cast_to_window(GLFWwindow* windowHandle) {
        return static_cast<GlfwWindow*>(glfwGetWindowUserPointer(windowHandle));
    }

}