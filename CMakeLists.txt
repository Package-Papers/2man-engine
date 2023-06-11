cmake_minimum_required(VERSION 3.22)
project(TwoManEngine LANGUAGES CXX)

# Generate the compilation database for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include dependencies
add_subdirectory(external)

# Include source
add_subdirectory(src)

# Link SFML
target_link_libraries(TwoManEngine PRIVATE sfml-system sfml-window sfml-audio)
target_compile_features(TwoManEngine PRIVATE cxx_std_20)

# Link SFML-IMGUI
target_link_libraries(TwoManEngine PRIVATE ImGui-SFML::ImGui-SFML)

# Including custom cmake rules
include(cmake/clang-cxx-dev-tools.cmake)

add_custom_target(dev)
add_dependencies(dev TwoManEngine clang-tidy clang-format)
