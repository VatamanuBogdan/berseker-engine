#!/bin/bash

CURRENT_SCRIPT_PATH=$(dirname $0)

source "${CURRENT_SCRIPT_PATH}/constants.env"

if [ -f "${BERSEKER_BINARY_PATH}" ]; then
    eval "${BERSEKER_BINARY_PATH}"
else
    echo "Executable not found" >&2
fi