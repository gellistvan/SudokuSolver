#!/bin/bash

original_dir=$(pwd)
build_dir=$original_dir"/build"
coverage_dir=$original_dir"/coverage_report"

enter_build_dir() {
    mkdir -p "$build_dir" && cd "$build_dir"
}

configure_project() {
    enter_build_dir
    cmake .. -DCMAKE_BUILD_TYPE=$BUILD_TYPE
}

build_project() {
    enter_build_dir
    cpu_cores=$(grep -c ^processor /proc/cpuinfo)
    make -j"$cpu_cores"
}

run_tests() {
    enter_build_dir
    ctest
}

generate_coverage_report() {
    enter_build_dir
    # Use gcovr to generate the coverage report, specifying the root directory and the output format
    mkdir -p "$coverage_dir"
    gcovr --root . --filter '.*modules.*' --exclude '.*test.*' --html --html-details -o "$coverage_dir/coverage_report.html"
    echo "Coverage report generated at $coverage_dir/coverage_report.html"
}

BUILD_TYPE="Release"

# Process options
while getopts "Cbgthj" opt; do
    case ${opt} in
        C )
            CONFIGURE=1
            ;;
        b )
            BUILD=1
            ;;
        g )
            BUILD_TYPE_PENDING=1
            ;;
        t )
            BUILD=1
            TEST=1
            ;;
        j )
            CONFIGURE=1
            BUILD=1
            TEST=1
            COVERAGE=1
            BUILD_TYPE="Debug"
            ;;
        h | * )
            echo "Usage: $0 [-C] [-b] [-g] [-t] [-j]"
            exit 1
            ;;
    esac
done

if [ ! -z "$CONFIGURE" ]; then
    configure_project
fi

if [ ! -z "$BUILD" ]; then
    build_project
fi

if [ ! -z "$TEST" ]; then
    run_tests
fi

if [ ! -z "$COVERAGE" ]; then
    generate_coverage_report
fi

cd "$original_dir"
