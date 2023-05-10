# !/bin/bash

workloadName=$1
myfilesize=$(wc -c ../ycsb/$workloadName | awk '{print $1}')
let threads=$2
let files=$threads
let split=$myfilesize/$files

cp simple_example.cc rocksdb/examples/
cp ../ycsb/$workloadName rocksdb/examples/
cd rocksdb/examples/
split -b $split $workloadName
echo "Completed Splitting"

make simple_example
echo "Completed Compiling"

collectl -i 0.1 -om -P -f $workloadName$threads &
PID=$!
cgexec -g memory:rocksdb ./simple_example $workloadName $threads
kill $PID