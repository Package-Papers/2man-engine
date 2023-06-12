# 2man-engine
A data oriented 2D game engine using SFML-GUI backend.

# Build Instructions
You can build via cmake. The target `dev` is bundled with clang-tidy and clang-format to enforce styles.
```sh
cmake -S . -B build
cmake --build build --target dev
``` 

# Contributions
> Note: Format your code with clang-format and clang-tidy.

This repository comes with a simple pre-commit configuration which runs clang-tidy and clang-format. To install the pre-commit script, you can simply run the following:

```sh
./install_precommit.sh
```
