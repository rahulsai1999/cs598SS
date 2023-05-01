#!/bin/bash

# Set output file name
OUTPUT_FILE1="disk_utilization.csv"
OUTPUT_FILE2="cpu_utilization.csv"
OUTPUT_FILE3="utilization.csv"

echo "Time,Read/s,Write/s,util%" > $OUTPUT_FILE1
echo "Time, cpu%" > $OUTPUT_FILE2
echo "Time,Read/s,Write/s,util%,cpu%" > $OUTPUT_FILE3

DISK_FILE="disk_stats.txt"

# Run iostat once and write to a temporary file
iostat -d -x 1 >> $DISK_FILE &

# Continuously monitor disk utilization using iostat and append data to the output file
while true; do
    # Get current time and format it as YYYY-MM-DD HH:MM:SS
    current_time=$(date '+%Y-%m-%d %H:%M:%S')
    # get cpu utilization using top
    cpu=$(mpstat 1 1 | awk '/^Average:/ {print 100-$NF}')
    echo "$current_time,$cpu" >> $OUTPUT_FILE2
    latest_disk_utilization=$(tail -2 "$DISK_FILE" | awk -W interactive '$1=="sda"{ print $2,",",$3,",",$NF;fflush();}')
    echo "$current_time,$latest_disk_utilization" >> $OUTPUT_FILE1
    echo "$current_time,$latest_disk_utilization,$cpu" >> $OUTPUT_FILE3
    # Get disk utilization using iostat and append data to the output file with timestamp
    sleep 1
done
