# !/bin/bash

workloadName=$1

rm wiredtiger/build/examples/c/ex_access/
rm -rf wiredtiger/build/examples/c/WT_HOME

mkdir ../data/wiredtiger/$workloadName
mv ../disk_utilization.csv ../data/wiredtiger/$workloadName
mv ../cpu_utilization.csv ../data/wiredtiger/$workloadName
mv ../utilization.csv ../data/wiredtiger/$workloadName
mv ../disk_stats.txt ../data/wiredtiger/$workloadName