#!/bin/bash

source=$0
dest=${0%.c}
gcc  -std=c99 -Wall -pedantic-errors -Werror -DNDEBUG $source -o $dest
