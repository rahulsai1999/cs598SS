# !/bin/bash

# Install CMake
sudo apt-get install -y cmake

# Install Swig
sudo apt-get install -y swig

git clone https://github.com/wiredtiger/wiredtiger.git
cd wiredtiger
mkdir build
cd build
cmake ../.
make
sudo make install
