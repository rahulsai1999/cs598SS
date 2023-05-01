# !/bin/bash

git clone https://github.com/vmware/splinterdb.git
cd splinterdb
git checkout 5aec241542e7c2ac7c5fc645d6ba89804c2b6277
wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | sudo apt-key add -
sudo add-apt-repository 'deb http://apt.llvm.org/focal/ llvm-toolchain-focal-13 main'
sudo apt-get install -y clang-13 clang-format-13 -y

export COMPILER=gcc    # or clang-13
sudo apt update -y
sudo apt install -y libaio-dev libconfig-dev libxxhash-dev $COMPILER
export CC=$COMPILER
export LD=$COMPILER
make
make run-tests
sudo make install