
# compile external dependencies
add_subdirectory(external)

# compile engine
add_library(Engine SHARED
        # Source
        "src/Application.cpp"
        "src/Window.cpp"
        "src/Input.cpp"
        "src/EngineConfig.cpp"

        # Core
        "include/Engine/Engine.h"
        "include/Engine/Core/Application.h"
        "include/Engine/Core/Core.h"
        "include/Engine/Core/Startup.h"
        "include/Engine/Core/Logger.h"
        "include/Engine/Core/Keys.h"
        "include/Engine/Core/Window.h"
        "include/Engine/Core/Input.h"
        "include/Engine/Core/EngineConfig.h"
        "include/Engine/Core/EngineConfigParser.h"
        
        # Events
        "include/Engine/Events/EventDispatcher.h"
        "include/Engine/Events/EventBase.h"
        "include/Engine/Events/EventCallback.h"
        "include/Engine/Events/FreeFunctionCallback.hpp"
        "include/Engine/Events/ICallback.hpp"
        "include/Engine/Events/Events.h"

        # Platform
        "include/Engine/Platform/GLFW/GlfwWindow.h" 
        "include/Engine/Platform/GLFW/GlfwWindow.cpp" 
        "include/Engine/Platform/GLFW/GlfwInput.h"
        "include/Engine/Platform/GLFW/GlfwInput.cpp"
        "include/Engine/Platform/Timer.h"
        "include/Engine/Platform/Timer.cpp" 

        # Utility
        "include/Engine/Utility/Singleton.h")
target_compile_features(Engine PUBLIC cxx_std_20)
target_include_directories(Engine PUBLIC "include/")
target_compile_definitions(Engine PRIVATE "$<$<CONFIG:Debug>:ENGINE_DEBUG>")
target_compile_definitions(Engine PRIVATE "ENGINE_DLL")
if (WIN32)
    target_compile_definitions(Engine PUBLIC "ENG_WINDOWS")
endif ()
if (UNIX)
    target_compile_definitions(Engine PUBLIC "ENG_LINUX")
endif()
target_link_libraries(Engine PUBLIC stb glad glfw glm imgui assimp spdlog)
