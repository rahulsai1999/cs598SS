# !/bin/bash

$workloadName=$1

cp c_simple_example.c rocksdb/examples/
cp ../ycsb/$workloadName rocksdb/examples/
cp ../ycsb/
cd rocksdb/examples/
make c_simple_example
./c_simple_example $workloadName