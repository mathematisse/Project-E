#!/bin/bash

HOOKS_DIR=$(git rev-parse --show-toplevel)/.git/hooks

SCRIPT_DIR=$(dirname "$(realpath "$0")")
cp "$SCRIPT_DIR/hook-clang.sh" "$HOOKS_DIR"/pre-commit
cp "$SCRIPT_DIR/hook-commit.sh" "$HOOKS_DIR"/commit-msg

chmod +x "$HOOKS_DIR"/pre-commit
chmod +x "$HOOKS_DIR"/commit-msg

echo "Git hooks installed successfully."
