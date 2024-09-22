#!/bin/bash

# Allowed verbs
allowed_verbs="Add|Edit|Remove|Document|Test|Refactor|Fix|Merge"

commit_message=$(head -n 1 "$1")

if [[ ! "$commit_message" =~ ^($allowed_verbs) ]]; then
    echo "Error: Commit message must start with one of the following verbs: $allowed_verbs"
    echo "Example: 'Add new feature' or 'Fix issue with X'"
    exit 1
fi

echo "Commit message is valid."
