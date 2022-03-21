#include "Engine/Core/Application.h"
#include "Engine/Core/Logger.h"
#include "Engine/Platform/Timer.h"
#include "Engine/Resources/Model.h"
#include "Engine/Resources/Shader.h"
#include <memory>

#if defined(ENG_WINDOWS) || defined(ENG_LINUX)

#include "Engine/Platform/GLFW/GlfwWindow.h"
#include "Engine/Platform/GLFW/GlfwInput.h"

#endif

namespace Eng {
    Application::Application() : window{nullptr}, input{nullptr}, isRunning{false} {
        app = this;
        window = GetWindow();
        window->CreateWindow("Application", 800, 400);
        input = GetInput();
        renderDevice = std::make_unique<RenderDevice>();
        renderer = std::make_unique<Renderer>();
        logger = Logger::GetLogger();
    };

    Application *Application::app = nullptr;

    Application::~Application() {
    }

    int Application::Start() {
        renderDevice->InitRenderDevice();
        isRunning = true;
        OnStart();

        glClearColor(52.0 / 255.0, 186.0 / 255.0, 235.0 / 255.0, 255.0 / 255.0);

        // load shaders
        Shader vShader = Shader::LoadFromDisk("resources/vModel.glsl");
        Shader fShader = Shader::LoadFromDisk("resources/fModel.glsl");
        auto shader_handle = renderDevice->CreateShaderProgram(vShader, fShader);

        // Load model
        Model model;
        model.LoadFromFile("resources/backpack/backpack.obj");

        auto model_handle = renderDevice->CreateModel(model);
        auto aspectRatio = (float) window->GetPixelWidth() / (float) window->GetPixelHeight();
        auto model_pos = Vec3{0.0, 0.0, -2.0};
        auto model_scale = Vec3{1.0, 1.0, 1.0};
        auto model_rotation = Vec3{0.0f, 0.0f, 1.0f};
        float rotation_angle = 0.0f;

        // Setup transform
        Transform t{model_pos, model_scale, model_rotation, rotation_angle};

        // Setup renderer
        Camera camera;
        renderer->SetCamera(camera);
        renderer->SetShader(shader_handle);

        const double ms_per_update = 50.0;
        Timer simulation_timer;
        simulation_timer.Start();
        double previous = simulation_timer.GetTime();
        double lag = 0.0;
        float camZ = 0.0f;
        float camX = 0.0f;
        while (window->IsOpen()) {
            double current = simulation_timer.GetTime();
            double elapsed = current - previous;
            previous = current;
            lag += elapsed;

            input->Update();
            OnUpdate();
            window->Update();
            camZ = cos(current / 1000) * 5.0;
            camX = sin(current / 1000) * 5.0;
            Vec3 cameraUp = Vec3{0.0, 1.0, 0.0};
            Vec3 cameraPos = Vec3{camX, 0.0, camZ};
            Vec3 cameraFront = model_pos;
            Vec3 cameraTarget = glm::normalize(model_pos - cameraPos);

            // Setup Camera
            camera.SetCameraPos({cameraPos, cameraTarget, cameraUp, -90.0f, 0.0f});
            camera.SetProjection(45, aspectRatio);

            lag -= ms_per_update;
            renderer->SetCamera(camera);

            // Render stuff
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            renderer->RenderModel(model_handle, t);
        }
        return 0;
    }

    Application *GetApplication() {
        return Application::app;
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
