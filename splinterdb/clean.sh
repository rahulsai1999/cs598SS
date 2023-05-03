# !/bin/bash

workloadName=$1

rm splinterdb/examples/$workloadName
rm splinterdb/examples/splinterdb_intro_db

mkdir ../data/splinterdb/$workloadName
mv ../disk_utilization.csv ../data/splinterdb/$workloadName
mv ../cpu_utilization.csv ../data/splinterdb/$workloadName
mv ../utilization.csv ../data/splinterdb/$workloadName
mv ../disk_stats.txt ../data/splinterdb/$workloadName

rm splinterdb/examples/xaa
rm splinterdb/examples/xab
rm splinterdb/examples/xac
rm splinterdb/examples/xad
rm splinterdb/examples/xae
rm splinterdb/examples/xaf
rm splinterdb/examples/xag
rm splinterdb/examples/xah
rm splinterdb/examples/xai
rm splinterdb/examples/xaj
rm splinterdb/examples/xak
rm splinterdb/examples/xal
rm splinterdb/examples/xam
rm splinterdb/examples/xan
rm splinterdb/examples/xao
rm splinterdb/examples/xap
rm splinterdb/examples/xaq
rm splinterdb/examples/xar
rm splinterdb/examples/xas
rm splinterdb/examples/xat
rm splinterdb/examples/xau
rm splinterdb/examples/xav
rm splinterdb/examples/xaw
rm splinterdb/examples/xax
rm splinterdb/examples/xay
rm splinterdb/examples/xaz