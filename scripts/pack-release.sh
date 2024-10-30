#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <version>"
    exit 1
fi
RELEASE_DIR=release/project-e-$1-linux

./scripts/check-version-name.sh $1 || exit 1

bash scripts/build-release.sh

# Create release folder
mkdir -p release
mkdir -p $RELEASE_DIR

# copy binaries & assets
cp cmake-build-release/libs/lib_ecs/liblib_ecs.a $RELEASE_DIR/project-e-$1-linux
cp cmake-build-release/libs/lib_net/liblib_net.a $RELEASE_DIR/project-e-$1-linux
cp cmake-build-release/libs/lib_log/liblib_log.a $RELEASE_DIR/project-e-$1-linux
cp cmake-build-release/modules/core/libcore.so $RELEASE_DIR/project-e-$1-linux
cp cmake-build-release/modules/render/librender.so $RELEASE_DIR/project-e-$1-linux
cp cmake-build-release/modules/spatial2d/libspatial2d.so $RELEASE_DIR/project-e-$1-linux
cp cmake-build-release/modules/network/libnetwork.so $RELEASE_DIR/project-e-$1-linux

rsync -av --include='*/' --include='*.hpp' --exclude='*' ./ $RELEASE_DIR/
# dele all empty folders in the release folder
find $RELEASE_DIR -type d -empty -delete

# Create tarball
tar -czf $RELEASE_DIR.tar.gz $RELEASE_DIR