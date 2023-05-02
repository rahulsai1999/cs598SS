#!/bin/bash

# Set output file name
NUM_CLIENTS=${1:-1}
OUTPUT_FILE="disk_throughput_${NUM_CLIENTS}.csv"

echo "Time, KB_Written" > $OUTPUT_FILE
# Run iostat once and write to a temporary file
iostat -d 1 | gawk '$1=="sda"{ print strftime("[%Y-%m-%d %H:%M:%S]"), $7;fflush() }'  >> $OUTPUT_FILE