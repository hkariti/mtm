#!/bin/bash

if [ $# == 0 ] || [ "$1" = "-h" ] || [ "$1" = "--help" ]; then
    echo "Usage: $0 FILE [FILE [...]]"
    echo Counts lines per C function
    exit 1
fi

awk  '/^[A-Za-z].*$/ {start=1; count=0; name=$0;} start == 1 && /{$/ {start = 0; middle=1} middle==1 { count+=1  } /^}$/ {middle=0; print count-2 ":", FILENAME ":", name}' "$@" | sort -n
