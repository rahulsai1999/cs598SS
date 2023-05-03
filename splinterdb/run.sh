# !/bin/bash

workloadName=$1
myfilesize=$(wc -c $workloadName | awk '{print $1}')
threads=$2
let split=$myfilesize/$threads

cp splinterdb_intro_example.c splinterdb/examples/
cp ../ycsb/ycsb-0.17.0/$workloadName splinterdb/examples/
cd splinterdb/examples/
split -b $split $workloadName
gcc splinterdb_intro_example.c -DSPLINTERDB_PLATFORM_DIR=platform_linux -lsplinterdb -lpthread
./a.out $threads