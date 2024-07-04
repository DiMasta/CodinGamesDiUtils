@echo off

REM Find and terminate any processes using port 8000
for /f "tokens=5" %%a in ('netstat -aon ^| findstr :8000 ^| findstr LISTENING') do (
    echo Terminating process with PID %%a using port 8000
    taskkill /PID %%a /F
)

REM Start a simple HTTP server (Python 3.x) in the current directory
start /B python -m http.server 8000

REM Wait for a few seconds to ensure the server starts
timeout /T 1

REM Open the default web browser and navigate to the URL
start http://localhost:8000/test_tree.html

REM Exit the batch script
exit
