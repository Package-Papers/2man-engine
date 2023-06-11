#!/bin/bash
echo "cmake --build build --target dev" > .git/hooks/pre-commit
chmod ug+x .git/hooks/*
echo "Pre-commit installed."