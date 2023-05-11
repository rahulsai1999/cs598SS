# !/bin/bash

git clone https://github.com/vmware/splinterdb.git
cp platform.c splinterdb/src/platform_linux/
cd splinterdb

wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | sudo apt-key add -
sudo add-apt-repository 'deb http://apt.llvm.org/focal/ llvm-toolchain-focal-13 main'

export COMPILER=gcc    # or clang-13
sudo apt update -y
sudo apt install -y libaio-dev libconfig-dev libxxhash-dev $COMPILER
export CC=$COMPILER
export LD=$COMPILER
make
make run-tests
sudo make install