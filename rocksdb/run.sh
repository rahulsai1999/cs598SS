# !/bin/bash

workloadName=$1
threads=$2

cp simple_example.cc rocksdb/examples/
cp ../ycsb/$workloadName rocksdb/examples/
cd rocksdb/examples/
make simple_example
./simple_example $workloadName $threads