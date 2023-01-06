@echo off

set INPUT_FILE=input_small_map_whole_game_48_tunrs.txt
set OUTPUT_FILE=input_small_map_whole_game_48_tunrs_extracted.txt
set DELIMITER=@
set REGEX_EXPRESSIONS="^Iterations:%DELIMITER%^Best Evaluation:%DELIMITER%^Standard Error Stream:$%DELIMITER%^\d+$%DELIMITER%^C\+\+$"

if not exist %INPUT_FILE% (
    color 0C
    echo ERROR: The given input file "%INPUT_FILE%" cannot be found! && color 0F
    exit /b
) else (
    python extract_script.py %INPUT_FILE% %OUTPUT_FILE% %DELIMITER% %REGEX_EXPRESSIONS%
    echo Script completed successfully!
)
