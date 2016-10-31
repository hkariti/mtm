#!/bin/bash

source=$1
dest=${1%.c}
gcc  -std=c99 -Wall -pedantic-errors -Werror -DNDEBUG $source -o $dest
