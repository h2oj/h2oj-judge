#!/bin/sh

set -o errexit

if [ $1 ]; then path=${1%*/};
else path="/opt/hoj-sandbox"; fi

if [ -d $path ]; then rm -r $path; fi
mkdir $path;
mkdir $path/rootfs;

# Mirrors:
# - Tsinghua: https://mirrors.tuna.tsinghua.edu.cn

if [ $2 ]; then mirror=${2%*/};
else mirror="http://dl-cdn.alpinelinux.org"; fi
download=$mirror"/alpine/v3.8/releases/x86_64/alpine-minirootfs-3.8.0-x86_64.tar.gz"

curl -L $download | tar -xzf - -C $path/rootfs
