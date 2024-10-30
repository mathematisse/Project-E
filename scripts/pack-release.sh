#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <version>"
    exit 1
fi
RELEASE_DIR=release/rtype-$1-x86_64-linux

./scripts/check-version-name.sh $1 || exit 1

bash scripts/build-release.sh

# Create release folder
mkdir -p release
mkdir -p $RELEASE_DIR

# copy binaries & assets
cp cmake-build-release/rtype/server/r-type_server $RELEASE_DIR/rtype-$1-server-x86_64-linux
cp cmake-build-release/rtype/client/r-type_client $RELEASE_DIR/rtype-$1-client-x86_64-linux
cp -r assets/ $RELEASE_DIR

# Create tarball
tar -czf $RELEASE_DIR.tar.gz $RELEASE_DIR