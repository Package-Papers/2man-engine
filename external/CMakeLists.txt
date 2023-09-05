# sfml
add_subdirectory(sfml)

# imgui
set(IMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/imgui")
set(IMGUI_SFML_FIND_SFML OFF)
set(IMGUI_SFML_IMGUI_DEMO ON)

# sfml-imgui
add_subdirectory(imgui-sfml)

# lua
option(LUA_BUILD_AS_CXX "Build lua as C++" ON)
add_subdirectory(lua)

