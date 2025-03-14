#!/usr/bin/env bash

# Find all relevant source files in the src directory
FILES=$(find src -type f \( -iname "*.cpp" -o -iname "*.hpp" -o -iname "*.cc" -o -iname "*.h" \))

if [ -z "$FILES" ]; then
	echo "No source files."
	exit 1
fi

echo "Running clang-format on source files..."
clang-format -i $FILES
echo "Done!"
