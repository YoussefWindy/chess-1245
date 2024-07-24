#!/bin/bash

# Check if input file is provided
if [ -z "$1" ]; then
    echo "Usage: $0 <input_file>"
    exit 1
fi

input_file="$1"

# Ensure the samples directory exists
rm -r samples
mkdir -p samples

# Read each line from the input file
while IFS= read -r stem; do
    in_file="tests/${stem}.in"
    out_file="samples/${stem}.txt"
    err_file="samples/${stem}.err"

    # Check if the input file exists
    if [ ! -f "$in_file" ]; then
        echo "Input file $in_file not found!"
        continue
    fi

    # Run the command and redirect stdout and stderr
    if ! ./chess < "$in_file" &> "$out_file"; then #  2> "$err_file"; then
        echo "Test $stem failed!"
    fi
done < "$input_file"

