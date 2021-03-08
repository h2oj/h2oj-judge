#!/bin/bash

set -o errexit

# Params:
#  $1: Sandbox directory path
#  $2: Mirror

if [ $1 ]; then sandbox_path=${1%*/};
else sandbox_path="/opt/hoj-sandbox"; fi

if [ -d $sandbox_path ]; then rm -r $sandbox_path; fi
mkdir $sandbox_path;
mkdir $sandbox_path/rootfs;

# Mirrors:
# - Alpine: http://dl-cdn.alpinelinux.org
# - Tsinghua: https://mirrors.tuna.tsinghua.edu.cn

if [ $2 ]; then mirror_name="${2}";
else mirror_name="alpine"; fi

if [ $mirror_name == "alpine" ]; then mirror_url="http://dl-cdn.alpinelinux.org";
elif [ $mirror_name == "tuna" ]; then mirror_url="https://mirrors.tuna.tsinghua.edu.cn";
elif [ $mirror_name == "custom" ]; then mirror_url=${3%*/}; fi

download=$mirror_url"/alpine/v3.13/releases/x86_64/alpine-minirootfs-3.13.2-x86_64.tar.gz"

curl -L $download | tar -xzf - -C $sandbox_path/rootfs

if [ $mirror_name == "tuna" ]; then
    sed -i 's/dl-cdn.alpinelinux.org/mirrors.tuna.tsinghua.edu.cn/g' $sandbox_path/rootfs/etc/apk/repositories
fi

# Set DNS
echo -e 'nameserver 8.8.8.8\nnameserver 8.8.4.4' > $sandbox_path/rootfs/etc/resolv.conf
