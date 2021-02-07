#!/bash/sh

cd ./third_party/hoj-testlib
if [ ! -d "bin" ]; then
    mkdir bin
fi
make
cd ../..
cp ./third_party/hoj-testlib/bin/noip build/hoj-checker
