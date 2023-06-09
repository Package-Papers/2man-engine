cmake_minimum_required(VERSION 3.21)
project(TwoManEngine LANGUAGES CXX)

# Include dependencies
add_subdirectory(external)

# Include source
add_subdirectory(src)

# Link libraries
target_link_libraries(TwoManEngine PRIVATE sfml-system sfml-window sfml-graphics sfml-audio)
target_compile_features(TwoManEngine PRIVATE cxx_std_20)
