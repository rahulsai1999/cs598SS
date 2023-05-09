# !/bin/bash

workloadName=$1

rm wiredtiger/build/examples/c/ex_access/ex_access
rm wiredtiger/build/examples/c/ex_access/$workloadName
rm -rf wiredtiger/build/examples/c/WT_HOME
rm -rf wiredtiger/build/examples/c/ex_access/WT_HOME

mkdir ../data/wiredtiger/$workloadName
mv ../disk_utilization.csv ../data/wiredtiger/$workloadName
mv ../cpu_utilization.csv ../data/wiredtiger/$workloadName
mv ../utilization.csv ../data/wiredtiger/$workloadName
mv ../disk_stats.txt ../data/wiredtiger/$workloadName

rm wiredtiger/build/examples/c/ex_access/xaa
rm wiredtiger/build/examples/c/ex_access/xab
rm wiredtiger/build/examples/c/ex_access/xac
rm wiredtiger/build/examples/c/ex_access/xad
rm wiredtiger/build/examples/c/ex_access/xae
rm wiredtiger/build/examples/c/ex_access/xaf
rm wiredtiger/build/examples/c/ex_access/xag
rm wiredtiger/build/examples/c/ex_access/xah
rm wiredtiger/build/examples/c/ex_access/xai
rm wiredtiger/build/examples/c/ex_access/xaj
rm wiredtiger/build/examples/c/ex_access/xak
rm wiredtiger/build/examples/c/ex_access/xal
rm wiredtiger/build/examples/c/ex_access/xam
rm wiredtiger/build/examples/c/ex_access/xan
rm wiredtiger/build/examples/c/ex_access/xao
rm wiredtiger/build/examples/c/ex_access/xap
rm wiredtiger/build/examples/c/ex_access/xaq
rm wiredtiger/build/examples/c/ex_access/xar
rm wiredtiger/build/examples/c/ex_access/xas
rm wiredtiger/build/examples/c/ex_access/xat
rm wiredtiger/build/examples/c/ex_access/xau
rm wiredtiger/build/examples/c/ex_access/xav
rm wiredtiger/build/examples/c/ex_access/xaw
rm wiredtiger/build/examples/c/ex_access/xax
rm wiredtiger/build/examples/c/ex_access/xay
rm wiredtiger/build/examples/c/ex_access/xaz