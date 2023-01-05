import argparse
import re

# Set up the argument parser
parser = argparse.ArgumentParser()
parser.add_argument("input_file", help="the name of the input file")
parser.add_argument("output_file", help="the name of the output file")
args = parser.parse_args()

# Set of regular expressions to match
patterns = [re.compile(r"^Standard Output Stream:$"), re.compile(r"^\b\d+\b")]

# Open the input file
with open(args.input_file, "r") as f:
    # Read the file line by line
    lines = f.readlines()

# Open the output file in write mode
with open(args.output_file, "w") as f:
    # Iterate over the lines
    for line in lines:
        # Check if the line matches any of the regular expressions
        if not any(pattern.match(line) for pattern in patterns):
            # Write the line to the output file
            f.write(line)
