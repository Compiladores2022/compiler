#!/bin/bash

file=$1
shift
filename="${file%.*}"
asm_file="$filename.s"

./npc $file
gcc $asm_file "$@" -o $filename
