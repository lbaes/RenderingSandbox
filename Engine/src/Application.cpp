#include "Engine/Core/Application.h"
#include "Engine/Core/Logger.h"
#include "Engine/Platform/Timer.h"
#include "Engine/Resources/Model.h"
#include "Engine/Resources/Shader.h"
#include <memory>

#if defined(ENG_WINDOWS) || defined(ENG_LINUX)

#include "Engine/Platform/GLFW/GlfwWindow.h"
#include "Engine/Platform/GLFW/GlfwInput.h"
#include "Engine/LLRenderer/OpenGL/GLRenderer.h"

#endif

#ifdef ENG_WINDOWS
typedef unsigned long DWORD;
extern "C" {
_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}
#endif

namespace Eng {
    Application::Application() : window{nullptr}, input{nullptr}, isRunning{false} {
        window = GetWindow();
        window->CreateWindow("Application", 1280, 720);
        input = GetInput();
        renderDevice = std::make_unique<RenderDevice>();
        renderer = std::make_unique<GLRenderer>();
        logger = Logger::GetLogger();
    };

    Application::~Application() {
    }

    int Application::Start() {
        renderDevice->InitRenderDevice();
        isRunning = true;
		// glClearColor(52.0 / 255.0, 186.0 / 255.0, 235.0 / 255.0, 255.0 / 255.0);
		OnStart();
        while (window->IsOpen()) {
			input->Update();
            OnUpdate();
			Draw();
            window->Update();
        }
        return 0;
    }

#ifdef ENG_WINDOWS

    Window *GetWindow() {
        static GlfwWindow window;
        return &window;
    }

    Input *GetInput() {
        auto window = dynamic_cast<GlfwWindow *>(GetWindow());
        static GlfwInput input(*window);
        return &input;
    }

#endif
}
