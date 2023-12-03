@echo off
echo Checking and unzipping data file...

if not exist actor-movie-data.csv (
    if exist actor-movie-data.zip (
        tar -xf actor-movie-data.zip
        echo Data file unzipped successfully.
    ) else (
        echo actor-movie-data.zip not found. Please ensure it is in the current directory.
        pause
        exit /b
    )
) else (
    echo Data file already exists.
)

echo Compiling C++ code...
g++ main.cpp -o main.exe
if %errorlevel% neq 0 pause

echo Starting Flask app...
start python app.py

echo Opening browser...
start http://127.0.0.1:5000

echo Done.