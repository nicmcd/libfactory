#!/bin/bash

set -e

BASE=$(realpath $(dirname "$0"))
BUILD_DIR=${BASE}/cmake-build
INSTALL_DIR=${BASE}/cmake-local

# tools during build
PKG_CONFIG_DIR=$(spack location -i pkg-config)
export PATH="${PKG_CONFIG_DIR}/bin:${PATH}"
CMAKE_DIR=$(spack location -i cmake)
CMAKE=${CMAKE_DIR}/bin/cmake

# dependencies

# paths
INSTALL_RPATH="${INSTALL_DIR}/lib;${INSTALL_DIR}/lib64"
PREFIX_PATH=""

mkdir -p ${BUILD_DIR}
cd ${BUILD_DIR} && ${CMAKE} \
    -G 'Unix Makefiles' \
    -DCMAKE_INSTALL_PREFIX:STRING=${INSTALL_DIR} \
    -DCMAKE_BUILD_TYPE:STRING=RelWithDebInfo \
    -DCMAKE_INTERPROCEDURAL_OPTIMIZATION:BOOL=OFF \
    -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON \
    -DCMAKE_INSTALL_RPATH_USE_LINK_PATH:BOOL=OFF \
    -DCMAKE_INSTALL_RPATH:STRING=${INSTALL_RPATH} \
    -DCMAKE_PREFIX_PATH:STRING=${PREFIX_PATH} \
    ..
cd ${BUILD_DIR} && make -j $(nproc) all && make install

echo ""
echo "Build successful :)"
echo ""
