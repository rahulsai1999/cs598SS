# !/bin/bash

workloadName=$1
threads=$2
let split=$threads+1

cp splinterdb_intro_example.c splinterdb/examples/
cp ../ycsb/ycsb-0.17.0/$workloadName splinterdb/examples/
cd splinterdb/examples/
split -n l/$split  outWorkloadA.txt
gcc splinterdb_intro_example.c -DSPLINTERDB_PLATFORM_DIR=platform_linux -lsplinterdb -lpthread
./a.out $threads