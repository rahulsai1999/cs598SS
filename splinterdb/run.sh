# !/bin/bash

workloadName=$1
threads=$2
total=1420
let split=$total/$threads

cp splinterdb_intro_example.c splinterdb/examples/
cp ../ycsb/ycsb-0.17.0/$workloadName splinterdb/examples/
split -b $split outWorkloadA.txt
cd splinterdb/examples/
gcc splinterdb_intro_example.c -DSPLINTERDB_PLATFORM_DIR=platform_linux -lsplinterdb -lpthread
./a.out $threads