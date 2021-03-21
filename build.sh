#!/bin/sh

set -o errexit

if [ \
    ! -d "third_party/hoj-testlib" -o \
    ! -d "third_party/argparse" -o \
    ! -d "third_party/yaml-cpp" \
]; then
    git submodule update --init --recursive
fi

#if [ ! -e "third_party/hoj-testlib/bin/noip" ]; then
#    cd third_party/hoj-testlib
#    if [ ! -d "bin" ]; then mkdir bin; fi
#    make
#    cd ../..
#fi

if [ ! -d "build" ]; then mkdir build; fi
cd build
cmake ..
make
cd ..

if [ ! -d "node/bin" ]; then mkdir node/bin; fi
cp build/hoj-judger node/bin/hoj-judger
