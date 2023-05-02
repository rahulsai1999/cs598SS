# !/bin/bash

workloadName=$1
collectl -i 0.01 -om -P -f $workloadName

