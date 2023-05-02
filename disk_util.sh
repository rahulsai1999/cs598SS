#!/bin/bash

# Set output file name
OUTPUT_FILE="disk_throughput.csv"

echo "Time, Bytes_Written" > $OUTPUT_FILE

DISK_FILE="disk_stats.txt"

# Run iostat once and write to a temporary file
iostat -d 1 | awk -W interactive -v date="$(date +"%Y-%m-%d %H:%M:%S")" '$1=="nvme0n1"{ print date, $NF;fflush();}' >> DISK_FILE
