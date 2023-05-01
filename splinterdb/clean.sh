# !/bin/bash

workloadName=$1

rm splinterdb/examples/$workloadName
rm splinterdb/splinterdb_intro_db

mkdir ../data/splinterdb/$workloadName
mv ../disk_utilization.csv ../data/splinterdb/$workloadName
mv ../cpu_utilization.csv ../data/splinterdb/$workloadName
mv ../utilization.csv ../data/splinterdb/$workloadName
mv ../disk_stats.txt ../data/splinterdb/$workloadName