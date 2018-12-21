#!/bin/bash

COMPILER="g++"
COMPILER_FLAGS="-std=c++11 -iquotesrc -Wall -Wextra \
	-Wfloat-equal -Wpointer-arith -Wcast-qual -Wunreachable-code"
DEBUG_FLAGS="-g -O0"
OPTIMIZE_FLAGS="-O3"
TARGET="finsim"
TARGET_BASE_FOLDER="./bin/"

if [ $# -eq 0 ]; then
	BUILD_TYPE="debug"
else
	BUILD_TYPE="$1"
fi

BUILD_MAIN=false
BUILD_TEST=false
RUN_TEST=false
if [ "${BUILD_TYPE}" = "debug" ]; then
	BUILD_MAIN=true
	EXTRA_FLAGS="${DEBUG_FLAGS}"
	TARGET_SUBFOLDER="debug/"
elif [ "${BUILD_TYPE}" = "test" ]; then
	BUILD_TEST=true
	RUN_TEST=true
	EXTRA_FLAGS="${DEBUG_FLAGS}"
	TARGET_SUBFOLDER="debug/"
elif [ "${BUILD_TYPE}" = "coverage" ]; then
	BUILD_MAIN=true
	BUILD_TEST=true
	EXTRA_FLAGS="${DEBUG_FLAGS} -fprofile-arcs -ftest-coverage"
	TARGET_SUBFOLDER="debug/"
elif [ "${BUILD_TYPE}" = "profile" ]; then
	BUILD_MAIN=true
	BUILD_TEST=true
	EXTRA_FLAGS="${OPTIMIZE_FLAGS} -pg"
	TARGET_SUBFOLDER="debug/"
elif [ "${BUILD_TYPE}" = "release" ]; then
	BUILD_MAIN=true
	EXTRA_FLAGS="${OPTIMIZE_FLAGS}"
	TARGET_SUBFOLDER="release/"
elif [ "${BUILD_TYPE}" = "clean" ]; then
	rm -rf ${TARGET_BASE_FOLDER}
	exit 0
else
	echo "Usage: ${0} [debug|test|coverage|profile|release|clean]"
	exit 1
fi

START_TIME=$(date -u +%s)
echo "Starting ${BUILD_TYPE} build"

find . -type f -not -path '*/\.*' | ./style.py

COMPILE_COMMAND="${COMPILER} ${COMPILER_FLAGS} ${EXTRA_FLAGS}"

MAIN_TARGET="${TARGET_BASE_FOLDER}${TARGET_SUBFOLDER}${TARGET}"
if [ "${BUILD_MAIN}" = true ]; then
	mkdir -p $(dirname ${MAIN_TARGET})
	${COMPILE_COMMAND} ./src/main.cpp -o ${MAIN_TARGET}
	if [ $? -ne 0 ]; then
		exit $?
	fi
fi

TEST_TARGET="${TARGET_BASE_FOLDER}${TARGET_SUBFOLDER}${TARGET}_test"
if [ "${BUILD_TEST}" = true ]; then
	mkdir -p $(dirname ${TEST_TARGET})
	${COMPILE_COMMAND} ./src/test/main.cpp -o ${TEST_TARGET}
	if [ $? -ne 0 ]; then
		exit $?
	fi
fi

if [ "${RUN_TEST}" = true ]; then
	${TEST_TARGET}
	if [ $? -ne 0 ]; then
		exit $?
	fi
fi

END_TIME=$(date -u +%s)
echo "Build completed successfully in $((${END_TIME}-${START_TIME})) seconds"
