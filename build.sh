#!/bin/bash

COMPILER="g++ -std=c++11 -iquotesrc -g -Wall -Wextra"
TARGET=finsim

if [ $# -eq 0 ] || [ "$1" = "debug" ]; then
	${COMPILER} ./src/main.cpp -o ./bin/debug/${TARGET}
elif [ "$1" = "test" ]; then
	TEST_TARGET="./bin/debug/${TARGET}_test"
	${COMPILER} ./src/test/main.cpp -o ${TEST_TARGET} && ${TEST_TARGET}
elif [ "$1" = "release" ]; then
	${COMPILER} -O3 ./src/main.cpp -o ./bin/release/${TARGET}
else
	echo "Usage: ${0} [debug|test|release]"
fi
