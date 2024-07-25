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
printf '=%.0s' {1..64}
echo
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
    if ./chess < "$in_file" &> "$out_file"; then
        status="PASSED!"
    else
        status="FAILED!"
    fi
    
    # Calculate the number of dashes
    test_name_length=${#stem}
    status_length=${#status}
    total_length=$((test_name_length + status_length))
    dash_length=$((63 - total_length - 6))  # 6 is for "TEST " prefix and space padding

    # Create the dashes
    dashes=$(printf '%*s' "$dash_length" '' | tr ' ' '-')

    # Print the test result with dashes
    printf "TEST %s %s %s\n" "$stem" "$dashes" "$status"
done < "$input_file"
printf '=%.0s' {1..64}
echo
