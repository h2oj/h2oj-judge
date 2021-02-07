#!/bin/sh

if [ -d "build" ]; then
    cp build/hoj-judger node/bin/hoj-judger
    cp build/hoj-checker node/bin/hoj-checker
fi
