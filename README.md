# 2man-engine
A data oriented 2D game engine using SFML-GUI backend.

# Build Instructions
You can build via cmake. The target `dev` is bundled with clang-tidy and clang-format to enforce styles.
```sh
cmake -S . -B build
cmake --build build --target dev
``` 

> Note: Documentation is still very lacking. 

# Features
- Entity Component System
- ECS based input system

