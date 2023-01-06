# import pdb; pdb.set_trace()
import argparse
import re

# Set up the argument parser
parser = argparse.ArgumentParser()
parser.add_argument("input_file", help="the name of the input file")
parser.add_argument("output_file", help="the name of the output file")
parser.add_argument("regexes_delimeter", help="the the charecter which splits the regexes")
parser.add_argument("regex_expressions", help="the regular expressions to match for lines, which will be deleted, separated by the given delimeter")
args = parser.parse_args()

# Split the list into individual regular expressions
regexes = args.regex_expressions.split("@")

# Compile the regular expressions
patterns = []
for regex in regexes:
    patterns.append(re.compile(regex))

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
