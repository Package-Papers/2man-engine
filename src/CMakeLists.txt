add_executable(TwoManEngine)

target_sources(TwoManEngine 
  PRIVATE
    mouse.cpp
    engine.cpp
    map/layer.cpp
    ecs/core/component.cpp
    ecs/core/component_pool.cpp
    ecs/core/entity_manager.cpp
    ecs/entity_capture.cpp
  PUBLIC 
    main.cpp
)

# set_target_properties(TwoManEngine PROPERTIES CXX_CLANG_TIDY "clang-tidy;-checks=*")

# Including custom cmake rules
if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET TwoManEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:TwoManEngine> $<TARGET_FILE_DIR:TwoManEngine> COMMAND_EXPAND_LISTS)
endif()