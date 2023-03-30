#!/bin/bash

# Create headers in the CSV file
echo "Timestamp,CPU Usage (%),Disk Usage (%)" > usage.csv

while true; do
    cpu=$(top -l 1 -n 1 -stats "cpu" | awk '/^CPU usage:/{print $3+$5}')
    disk=$(df -h | awk 'NR==2{print $5}')
    timestamp=$(date +%s)
    echo "$timestamp,$cpu%,$disk" >> usage.csv
    sleep 0.01
done