# !/bin/bash

workloadName=$1

cp ex_access.c wiredtiger/examples/c/
cp ../ycsb/ycsb-0.17.0/$workloadName wiredtiger/build/examples/c/ex_access/
cd wiredtiger/build/examples/c
make ex_access
./ex_access/ex_access $workloadName