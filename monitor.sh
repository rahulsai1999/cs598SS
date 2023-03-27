#!/bin/bash

# Create headers in the CSV file
echo "Date and Time,CPU Usage (%),Disk Usage (%)" > usage.csv

while true; do
    cpu=$(top -l 1 -n 1 -stats "cpu" | awk '/^CPU usage:/{print $3+$5}')
    disk=$(df -h | awk 'NR==2{print $5}')
    timestamp=$(date +"%Y/%m/%d %H:%M:%S")
    echo "$timestamp,$cpu%,$disk" >> usage.csv
    sleep 2
done
