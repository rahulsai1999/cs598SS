# !/bin/bash

workloadName=$1

rm rocksdb/examples/$workloadName
rm -rf rocksdb/examples/rocksdb_simple_example
rm -rf rocksdb/examples/rocksdb_simple_example_backup

mkdir ../data/rocksdb/$workloadName
mv ../disk_utilization.csv ../data/rocksdb/$workloadName
mv ../cpu_utilization.csv ../data/rocksdb/$workloadName
mv ../utilization.csv ../data/rocksdb/$workloadName
mv ../disk_stats.txt ../data/rocksdb/$workloadName

rm rocksdb/examples/xaa
rm rocksdb/examples/xab
rm rocksdb/examples/xac
rm rocksdb/examples/xad
rm rocksdb/examples/xae
rm rocksdb/examples/xaf
rm rocksdb/examples/xag
rm rocksdb/examples/xah
rm rocksdb/examples/xai
rm rocksdb/examples/xaj
rm rocksdb/examples/xak
rm rocksdb/examples/xal
rm rocksdb/examples/xam
rm rocksdb/examples/xan
rm rocksdb/examples/xao
rm rocksdb/examples/xap
rm rocksdb/examples/xaq
rm rocksdb/examples/xar
rm rocksdb/examples/xas
rm rocksdb/examples/xat
rm rocksdb/examples/xau
rm rocksdb/examples/xav
rm rocksdb/examples/xaw
rm rocksdb/examples/xax
rm rocksdb/examples/xay
rm rocksdb/examples/xaz