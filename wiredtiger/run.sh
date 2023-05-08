# !/bin/bash

workloadName=$1

cp ex_access.c wiredtiger/examples/c/
cp ../ycsb/$workloadName wiredtiger/build/examples/c/ex_access/
cd wiredtiger/build/examples/c
make ex_access
cd ex_access
./ex_access $workloadName