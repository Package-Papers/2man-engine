add_executable(TwoManEngine)
target_sources(TwoManEngine 
  PRIVATE
    engine.cpp
  PUBLIC 
    main.cpp
)
# set_target_properties(TwoManEngine PROPERTIES CXX_CLANG_TIDY "clang-tidy;-checks=*")