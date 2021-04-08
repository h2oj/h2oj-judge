#!/bin/sh

set -o errexit

git submodule update --init --recursive

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
cp build/h2oj-judge node/bin/h2oj-judge
