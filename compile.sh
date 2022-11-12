#!/bin/bash

file=$1
filename="${file%.*}"
asm_file="$filename.s"

if [ ! -f ./npc ]; then
    make npc
fi

./npc $file
if [ $? -eq 0 ]; then
    if [ $# -eq 2 ]; then
        gcc $asm_file $2 -o $filename
    else
        gcc $asm_file lib.c -o $filename
    fi
   ./$filename
fi
