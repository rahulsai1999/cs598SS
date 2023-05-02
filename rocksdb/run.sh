# !/bin/bash

workloadName=$1

cp simple_example.cc rocksdb/examples/
cp ../ycsb/ycsb-0.17.0/$workloadName rocksdb/examples/
cd rocksdb/examples/
make simple_example
./simple_example $workloadName