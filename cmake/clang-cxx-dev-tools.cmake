# Referenced from: https://www.labri.fr/perso/fleury/posts/programming/using-clang-tidy-and-clang-format.html

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

add_custom_target(dev)

# Adding clang-format target if executable is found
find_program(CLANG_FORMAT "clang-format")
if(CLANG_FORMAT)
  add_custom_target(
    clang-format
    COMMAND /usr/bin/clang-format
    -i
    -style=file
    ${ALL_CXX_SOURCE_FILES}
    )
  
  add_dependencies(dev TwoManEngine clang-format)
endif()

# Adding clang-tidy target if executable is found
find_program(CLANG_TIDY "clang-tidy")
if(CLANG_TIDY)
  add_custom_target(
    clang-tidy
    COMMAND /usr/bin/clang-tidy
    ${ALL_CXX_SOURCE_FILES}
    --fix
    --
    -isystem ${PROJECT_SOURCE_DIR}/external/sfml/
    -isystem ${PROJECT_SOURCE_DIR}/external/imgui/
    -isystem ${PROJECT_SOURCE_DIR}/external/imgui-sfml/
    -std=c++17
    )
  
  add_dependencies(dev TwoManEngine clang-tidy)
endif()



        
