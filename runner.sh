#!/bin/bash
SOURCE_FILE="hello_world.cpp"
EXECUTABLE="hello_world"

g++ "$SOURCE_FILE" -o "$EXECUTABLE"

if [ $? -eq 0 ]; then
    echo "Compilation successful. Running the program:"
    ./"$EXECUTABLE"
else
    echo "Compilation failed."
fi