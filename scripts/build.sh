#!/bin/bash

CURRENT_SCRIPT_PATH=$(dirname $0)
CURRENT_WORKING_PATH=$(realpath .)

source "${CURRENT_SCRIPT_PATH}/constants.env"

BUILD_TYPE="$1"

if [[ "$BUILD_TYPE" == "release" ]]; then
    CMAKE_BUILD_TYPE="Release"
elif [[ "$BUILD_TYPE" == "debug" || "$BUILD_TYPE" == "" ]]; then
    CMAKE_BUILD_TYPE="Debug"
else
    echo -e "Invalid '${BUILD_TYPE}' build type. The available ones are 'release' and 'debug'" >&2
    exit -1
fi

if [ -d "$BUILD_DIRECTORY_PATH" ]; then
    mkdir -p "$BUILD_DIRECTORY_PATH"
fi

cmake -B "$BUILD_DIRECTORY_PATH" \
      -S "$CURRENT_WORKING_PATH" \
      -G "$CMAKE_GENERATOR" \
      -D "CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"

if [ ! $? -eq 0 ]; then
    echo "CMake generation failed" >&2
    exit -1
fi

CPU_CORES_COUNT=$(echo -n $(nproc --all))
if [ ! $? -eq 0 ]; then
    echo "Failed to determine the number of CPU cores that is used to improve the build performance" >&2
    CPU_CORES_COUNT=1
fi

make -j "$CPU_CORES_COUNT" -C "$BUILD_DIRECTORY_PATH"
