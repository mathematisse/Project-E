#!/bin/bash

if ! command -v clang-format &> /dev/null; then
    echo "clang-format not found, attempting to install..."
    if [[ "$OSTYPE" == "linux-gnu"* ]]; then
        sudo apt install clang-format -y
    elif [[ "$OSTYPE" == "darwin"* ]]; then
        brew install clang-format
    else
        echo "Please install clang-format manually."
        exit 1
    fi
fi

if ! command -v clang-tidy &> /dev/null; then
    echo "clang-tidy not found, attempting to install..."
    if [[ "$OSTYPE" == "linux-gnu"* ]]; then
        sudo apt install clang-tidy -y
    elif [[ "$OSTYPE" == "darwin"* ]]; then
        brew install clang-tidy
    else
        echo "Please install clang-tidy manually."
        exit 1
    fi
fi

echo "Running clang-format..."
git ls-files -o -c --exclude-standard | grep -E '\.(cpp|hpp|cc|c|h)$' | xargs clang-format -i
if ! git diff --quiet; then
    echo "clang-format made changes. Please review and commit the changes."
    exit 1
fi

# echo "Running clang-tidy..."
# for file in $(git diff --cached --name-only --diff-filter=d | grep -E '\.(cpp|cc|c|h)$'); do
#     clang-tidy $file --fix --fix-errors
# done
#     # git add $file

echo "Pre-commit hook done."
