# !/bin/bash

workloadName=$1
myfilesize=$(wc -c ../ycsb/$workloadName | awk '{print $1}')
let threads=$2
let files=$threads
let split=$myfilesize/$files

cp ex_access.c wiredtiger/examples/c/
cp ../ycsb/$workloadName wiredtiger/build/examples/c/ex_access/
cd wiredtiger/build/examples/c/ex_access/
split -b $split $workloadName
cd ..
make ex_access
echo "Completed Compilation"

collectl -i 0.1 -om -P -f $workloadName$threads &
PID=$!
cd ex_access
./ex_access $workloadName $threads
kill $PID