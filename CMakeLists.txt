cmake_minimum_required(VERSION 3.22)
project(TwoManEngine LANGUAGES CXX)

# Generate the compilation database for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include dependencies
add_subdirectory(external)

# CPM
include(cmake/CPM.cmake)

CPMAddPackage(
  NAME Observe
  VERSION 3.0
  GITHUB_REPOSITORY TheLartians/Observe
)

# Include source
add_subdirectory(src)
set(CMAKE_CXX_STANDARD 23)

# Include rapidjson
target_include_directories(TwoManEngine PUBLIC
  ${PROJECT_SOURCE_DIR}/external/rapidjson
)

# Link SFML
target_link_libraries(TwoManEngine PRIVATE sfml-system sfml-window sfml-audio Observe)

# Link SFML-IMGUI
target_link_libraries(TwoManEngine PRIVATE ImGui-SFML::ImGui-SFML)


if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(MACOSX TRUE)
endif()


if (${MACOSX})

  # Additional targets to perform clang-format/clang-tidy
  # Get all project files
  file(GLOB_RECURSE ALL_CXX_SOURCE_FILES
       ${PROJECT_SOURCE_DIR}/src/*.[chi]pp
       ${PROJECT_SOURCE_DIR}/src/*.[chi]xx
       ${PROJECT_SOURCE_DIR}/src/*.cc
       ${PROJECT_SOURCE_DIR}/src/*.hh
       ${PROJECT_SOURCE_DIR}/src/*.ii
       ${PROJECT_SOURCE_DIR}/src/*.[CHI]
  )

  add_custom_target(dev ALL)

  # Adding clang-format target if executable is found
  find_program(CLANG_FORMAT "clang-format")
  if(CLANG_FORMAT)
    add_custom_command(TARGET dev
      COMMAND /usr/bin/clang-format
      -i
      -style=file
      ${ALL_CXX_SOURCE_FILES}
      )
  endif()

  # Adding clang-tidy target if executable is found
  find_program(CLANG_TIDY "clang-tidy")
  if(CLANG_TIDY)
    add_custom_command(TARGET dev
      COMMAND /usr/bin/clang-tidy
      ${ALL_CXX_SOURCE_FILES}
      --fix
      --
      -isystem ${PROJECT_SOURCE_DIR}/external/sfml/
      -isystem ${PROJECT_SOURCE_DIR}/external/imgui/
      -isystem ${PROJECT_SOURCE_DIR}/external/imgui-sfml/
      -isystem ${PROJECT_SOURCE_DIR}/external/rapidjson/
      -std=c++17
      )
  endif()
else()
    # Including custom cmake rules
  include(cmake/clang-cxx-dev-tools.cmake)
endif()

