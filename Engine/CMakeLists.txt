
# compile external dependencies
add_subdirectory(external)

# compile engine
add_library(Engine SHARED
        # Source
        "src/Application.cpp"
        "src/Window.cpp"
        "src/Input.cpp"
        "src/EngineConfig.cpp"

        # Platform
        "include/Engine/Platform/GLFW/GlfwWindow.h" 
        "include/Engine/Platform/GLFW/GlfwWindow.cpp" 
        "include/Engine/Platform/GLFW/GlfwInput.h"
        "include/Engine/Platform/GLFW/GlfwInput.cpp"
        "include/Engine/Platform/Timer.h"
        "include/Engine/Platform/Timer.cpp" 

        # Types
        "include/Engine/Core/Types/Vec2.h" 
        "include/Engine/Core/Types/Vec3.h" 
        "include/Engine/Core/Types/Vec4.h" 
        "include/Engine/Core/Types/Mat2.h" 
        "include/Engine/Core/Types/Mat3.h" 
        "include/Engine/Core/Types/Mat4.h" 
        "include/Engine/Core/Types/Mat.h" 
        "include/Engine/Core/Types/Vec.h"
        "include/Engine/Core/Types/Color.h"
        "include/Engine/Core/Types/HashedString.h"

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

        # Resources
        "include/Engine/Resources/ResourceManager.h"
        "include/Engine/Resources/ResourceCache.h"
        "include/Engine/Resources/ResourceHandle.h"
        "include/Engine/Resources/ManagedResources.h"
        "include/Engine/Resources/ImageLoader.h"
        "include/Engine/Resources/ImageLoader.cpp" 
        "include/Engine/Resources/Image.h"
        "include/Engine/Resources/Texture2D.h" 
        "include/Engine/Resources/Texture2D.cpp"
        "include/Engine/Resources/Material.h" 
        "include/Engine/Resources/Font.h"
        "include/Engine/Resources/Shader.h"
        "include/Engine/Resources/Shader.cpp"
        "include/Engine/Resources/Mesh.h"

        # LL Renderer
        
        "include/Engine/LLRenderer/Vertex.h"
        "include/Engine/LLRenderer/Buffers.h"
        "include/Engine/LLRenderer/RenderDevice.h"
        "include/Engine/LLRenderer/RenderDevice.cpp"
        "include/Engine/LLRenderer/ErrorLogger.h" 
        "include/Engine/LLRenderer/Handles.h"

        # Utility
        "include/Engine/Utility/Singleton.h"  
        "include/Engine/Utility/FlagEnum.h"    
        "include/Engine/Utility/ReadFile.h"    
        "include/Engine/Utility/ReadFile.cpp"        
        "include/Engine/Utility/TypeString.h" 
   )
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
target_link_libraries(Engine PUBLIC stb glad glfw glm imgui assimp spdlog murmurhash)
