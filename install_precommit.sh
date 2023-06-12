#!/bin/bash

> .git/hooks/pre-commit
echo "#!/bin/bash" >> .git/hooks/pre-commit
echo "CXX_CHANGED_FILES=\$(git diff --cached --name-only --diff-filter=ACM | grep -Ee '\.([chi](pp|xx)|(cc|hh|ii)|[CHI])\$')" >> .git/hooks/pre-commit
echo "if [[ -n "\$CXX_CHANGED_FILES" ]]" >> .git/hooks/pre-commit
echo "then" >> .git/hooks/pre-commit
echo "	/usr/bin/clang-format -i -style=file \${CXX_CHANGED_FILES}" >> .git/hooks/pre-commit
echo "	/usr/bin/clang-tidy --fix \${CXX_CHANGED_FILES} -- -isystem external/sfml -isystem external/imgui -isystem external/imgui-sfml" >> .git/hooks/pre-commit
echo "else" >> .git/hooks/pre-commit
echo "	echo 'Pre-commit: Nothing to be done.'" >> .git/hooks/pre-commit
echo "fi" >> .git/hooks/pre-commit

chmod ug+x .git/hooks/*
echo "Pre-commit installed."