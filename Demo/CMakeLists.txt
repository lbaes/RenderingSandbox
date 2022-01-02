add_executable (Demo "src/Demo.cpp" "include/Demo.h")
target_compile_features(Demo PUBLIC cxx_std_20)
target_include_directories(Demo PRIVATE "include/")
target_link_libraries(Demo PUBLIC Engine)
add_custom_target(COPY_ENGINE_DLL
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:Demo> $<TARGET_FILE_DIR:Demo>
  COMMAND_EXPAND_LISTS
  )
add_dependencies(COPY_ENGINE_DLL Engine)
add_dependencies(Demo COPY_ENGINE_DLL)