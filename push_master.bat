@echo off

rem Find the root directory of the git repository
for /f "delims=" %%a in ('git rev-parse --show-toplevel 2^>^&1') do set "root_dir=%%a"

rem Check if the command succeeded
if %errorlevel% neq 0 (
    rem Display an error message if the command failed
    echo Error: Not a git repository
    exit /b 1
)

rem Change to the root directory of the repository
cd /d "%root_dir%"

rem Ask the user for a commit message
set /p commit_message=Enter commit message: 

rem Stash the current changes
git stash

rem Pull the latest changes from the HEAD
git pull

rem Pop the stash and commit the changes with the given commit message
git stash pop
git add .
git commit -am "%commit_message%"

rem Push the changes to the remote repository
git push
