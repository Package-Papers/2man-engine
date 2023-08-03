add_executable(TwoManEngine)

target_sources(TwoManEngine 
  PRIVATE
    engine.cpp
    mouse.cpp
    ecs/core/component.cpp
    ecs/core/component_pool.cpp
  PUBLIC 
    main.cpp
)

# set_target_properties(TwoManEngine PROPERTIES CXX_CLANG_TIDY "clang-tidy;-checks=*")

# Including custom cmake rules
if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET TwoManEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:TwoManEngine> $<TARGET_FILE_DIR:TwoManEngine> COMMAND_EXPAND_LISTS)
endif()