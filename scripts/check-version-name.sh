#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <version>"
    exit 1
fi

if [[ "$1" =~ ^[0-9]+\.[0-9]+\.[0-9]+$ ]]; then
    echo "PR title is a valid version number."
else
    echo "PR title \"$1\" is not a valid version number."
    echo "Please use the format x.y.z"
    exit 1
fi
