#!/bin/bash

# Set output file name
OUTPUT_FILE="disk_throughput.csv"

echo "Time, KB_Written" > $OUTPUT_FILE
# Run iostat once and write to a temporary file
iostat -d 1 | gawk '$1=="nvme0n1"{ print strftime("[%Y-%m-%d %H:%M:%S]"), $NF;fflush() }'  >> $OUTPUT_FILE