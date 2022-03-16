add_executable (Demo "src/Demo.cpp" "include/Demo.h")
target_compile_features(Demo PUBLIC cxx_std_20)
target_include_directories(Demo PRIVATE "include/")
target_link_libraries(Demo PUBLIC Engine)
add_custom_target(COPY_ENGINE_DLL
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:Demo> $<TARGET_FILE_DIR:Demo>
  COMMAND_EXPAND_LISTS
  )
add_custom_target(COPY_MODELS
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Models/ $<TARGET_FILE_DIR:Demo>/resources
  COMMAND_EXPAND_LISTS
  )
add_custom_target(COPY_SHADERS
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Shaders/ $<TARGET_FILE_DIR:Demo>/resources
  COMMAND_EXPAND_LISTS
  )
add_dependencies(COPY_ENGINE_DLL Engine)
add_dependencies(Demo COPY_ENGINE_DLL COPY_MODELS COPY_SHADERS)