# !/bin/bash

workloadName=$1
myfilesize=$(wc -c ../ycsb/$workloadName | awk '{print $1}')
let threads=$2
let files=$threads
let split=$myfilesize/$files

cp splinterdb_intro_example.c splinterdb/examples/
cp ../ycsb/$workloadName splinterdb/examples/
cd splinterdb/examples/
split -b $split $workloadName
gcc splinterdb_intro_example.c -DSPLINTERDB_PLATFORM_DIR=platform_linux -lsplinterdb -lpthread
./a.out $threads