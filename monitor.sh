#!/bin/bash

# Set output file name
OUTPUT_FILE="disk_utilization.csv"

echo "Time,Read/s,Write/s,util%" > $OUTPUT_FILE

# Continuously monitor disk utilization using iostat and append data to the output file
while true; do
    # Get current time and format it as YYYY-MM-DD HH:MM:SS
    current_time=$(date '+%Y-%m-%d %H:%M:%S')
    
    # Get disk utilization using iostat and append data to the output file with timestamp
    iostat -d -x 1 | awk -v time="$current_time" '$1=="nvme0n1"{ print time",",$2,",",$3,",",$NF;fflush();}' >> $OUTPUT_FILE
    
    # Wait for 1 second before checking again
    sleep 1
done