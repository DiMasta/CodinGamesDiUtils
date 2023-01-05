@echo off

set INPUT_FILE=moves.txt
set OUTPUT_FILE=moves_only.txt

if not exist %INPUT_FILE% (
    echo ERROR: The given input file "%INPUT_FILE%" cannot be found!
    exit /b
)

python extract_script.py moves.txt moves_only.txt

echo Script completed successfully!
