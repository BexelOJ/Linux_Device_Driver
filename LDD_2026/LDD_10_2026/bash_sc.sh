#!/bin/bash

for file in *.c; do
    dir="${file%.c}"
    mkdir -p "$dir"
    mv "$file" "$dir/"
done
