#!/bin/bash

# This script calls ntldd to find the DLL dependencies of a given Windows executable
# and copies those DLLs into the same directory as the executable.
# Only look for MSYS2 DLLs dependencies.
# Usage: ./ntldd-copy.sh <path-to-executable>
# Example: ./ntldd-copy.sh /path/to/myapp.exe
# Note: This script requires ntldd to be installed and available in the system PATH.

bin_exe="$1"

if [ -z "$bin_exe" ]; then
	echo "Usage: $0 <path-to-executable>"
	exit 1
fi

if [ ! -f "$bin_exe" ]; then
	echo "Error: File '$bin_exe' not found!"
	exit 1
fi

if [ ! -x "$bin_exe" ]; then
	echo "Error: File '$bin_exe' is not executable!"
	exit 1
fi

bin_dir="$(dirname "$bin_exe")"
bin_name="$(basename "$bin_exe")"

echo "Finding dependencies for '$bin_name'..."

deps=$(ntldd -R "$bin_exe" | grep -i msys | grep '=>' | awk '{print $3}' | grep -E '\.dll$' | sort -u)

if [ -z "$deps" ]; then
	echo "No dependencies found."
	exit 0
fi

echo "Dependencies found:"

for dep in $deps; do
	if [ -f "$dep" ]; then
		echo "Copying '$dep' to '$bin_dir'..."
		cp "$dep" "$bin_dir/"
	else
		echo "Warning: Dependency '$dep' not found!"
	fi
done

echo "All dependencies copied."