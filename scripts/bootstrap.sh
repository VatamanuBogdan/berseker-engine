#!/bin/bash

export BERSEKER_PROJECT_PATH="$(realpath "$(dirname $0)/..")"
export BERSEKER_SCRIPTS_PATH="${BERSEKER_PROJECT_PATH}/scripts"

function __check_and_run_berseker_script() {
    
    if [[ "$BERSEKER_PROJECT_PATH" != "$(realpath .)" ]] then;
        echo "Invalid working directory you must be at ${BERSEKER_PROJECT_PATH} to run this command" >&2
        exit -1
    fi

    script_path="${BERSEKER_SCRIPTS_PATH}/$1"

    if [ -f $script_path ]; then
        eval $script_path "${@:2}"
    else
        echo -e "Invalid path to script ${script_path}. \nIf the project was moved re-run 'source bootstrap.sh'" >&2
    fi
}

function berseker_build() {
    __check_and_run_berseker_script build.sh "$@"
}

function berseker_cleanup() {
    __check_and_run_berseker_script cleanup.sh "$@"
}

function berseker_run() {
    __check_and_run_berseker_script run.sh "$@"
}

function berseker_reload_shaders() {
    __check_and_run_berseker_script shader_resources_generator.py "$@"
}