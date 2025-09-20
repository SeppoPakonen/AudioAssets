#!/bin/bash
mkdir tmp
cd tmp

mkdir all_tg
mkdir converted

rm all_tg/*
rm converted/*

find ../music/Composed -type f -print0 | 
while IFS= read -r -d '' file; do
        parentdir="$(dirname "$file")"
        parentdir="$(basename "$parentdir")"
        filename="$(basename "$file")"
        #printf '%s\n' "$file"
        #echo $filename
        ln "$file" "all_tg/${parentdir}_${filename}"
done

find ../music/Release -type f -print0 | 
while IFS= read -r -d '' file; do
        parentdir="$(dirname "$file")"
        parentdir="$(basename "$parentdir")"
        filename="$(basename "$file")"
        #printf '%s\n' "$file"
        #echo $filename
        ln "$file" "all_tg/${parentdir}_${filename}"
done


