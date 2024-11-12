#!/bin/bash

CURRENT_SCRIPT_PATH=$(dirname $0)

source "${CURRENT_SCRIPT_PATH}/constants.env"

if [ -d "$BUILD_DIRECTORY_PATH" ]; then
    rm -rf "$BUILD_DIRECTORY_PATH"
fi