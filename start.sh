#!/bin/bash

echo "Checking and unzipping data file..."

if [[ ! -f actor-movie-data.csv ]]; then
    if [[ -f actor-movie-data.zip ]]; then
        unzip actor-movie-data.zip
        echo "Data file unzipped successfully."
    else
        echo "actor-movie-data.zip not found. Please ensure it is in the current directory."
        exit 1
    fi
else
    echo "Data file already exists."
fi

echo "Compiling C++ code..."
g++ -std=c++11 main.cpp -o main
if [[ $? -ne 0 ]]; then
    echo "Compilation failed."
    exit 1
fi

echo "Starting Flask app..."
python3 app.py &

echo "Opening browser..."
open http://127.0.0.1:5000

echo "Done."
