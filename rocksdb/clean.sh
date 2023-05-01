# !/bin/bash

workloadName=$1

rm rocksdb/examples/$workloadName
rm -rf rocksdb/examples/rocksdb_c_simple_example
rm -rf rocksdb/examples/rocksdb_c_simple_example_backup

mkdir ../data/rocksdb/$workloadName
mv ../disk_utilization.csv ../data/rocksdb/$workloadName
mv ../cpu_utilization.csv ../data/rocksdb/$workloadName
mv ../utilization.csv ../data/rocksdb/$workloadName
mv ../disk_stats.txt ../data/rocksdb/$workloadName