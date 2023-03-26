#!/bin/bash

# create a header row for the CSV file
echo "timestamp,cpu_usage,disk_usage" > utilization.csv

# function to monitor CPU utilization
monitor_cpu() {
  while true
  do
    timestamp=$(date +%s)
    cpu_usage=$(top -n 1 | awk '/%Cpu/ {print $2}')
    disk_usage=$(df -h | awk '$NF=="/"{printf "%s\n", $5}')
    echo "$timestamp,$cpu_usage,$disk_usage" >> utilization.csv
    sleep 5
  done
}

# start the monitoring function in the background
monitor_cpu &

# wait for the monitoring to finish
wait
