# !/bin/bash

# Install Java and Maven
sudo apt-get update
sudo apt-get install -y openjdk-8-jdk maven

# Check Java version
java -version

# Setup YCSB
curl -O --location https://github.com/brianfrankcooper/YCSB/releases/download/0.17.0/ycsb-0.17.0.tar.gz
tar xfvz ycsb-0.17.0.tar.gz
rm ycsb-0.17.0.tar.gz

# Install WiredTiger
sudo apt-get update
sudo apt-get install -y wiredtiger

# Install RocksDB
sudo apt-get install build-essential libsnappy-dev zlib1g-dev libbz2-dev libgflags-dev
git clone https://github.com/facebook/rocksdb.git
cd rocksdb
cd examples/; make all
cd ..

# Install SplinterDB
git clone https://github.com/vmware/splinterdb.git
cd splinterdb
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

# mkdir -p /usr/local/include/splinterdb /usr/local/lib
# cp -p build/release/lib/libsplinterdb.so build/release/lib/libsplinterdb.a /usr/local/lib
# cp -p -r include/splinterdb/ /usr/local/include/

# export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib
# gcc splinterdb_intro_example.c -DSPLINTERDB_PLATFORM_DIR=platform_linux -l splinterdb
# ./a.out