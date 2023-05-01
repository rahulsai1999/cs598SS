# !/bin/bash

$workloadName=$1

cp c_simple_example.c rocksdb/examples/
cp ../ycsb/ycsb-0.17.0/$workloadName rocksdb/examples/
cd rocksdb/examples/
make c_simple_example
./c_simple_example $workloadName