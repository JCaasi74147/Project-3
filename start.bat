@echo off
echo Compiling C++ code...
g++ main.cpp -o main.exe
if %errorlevel% neq 0 pause

echo Starting Flask app...
start python app.py

echo Opening browser...
start http://127.0.0.1:5000

echo Done.