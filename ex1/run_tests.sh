#!/bin/bash

cd PokemonGo
export PATH=$PATH:../scripts
for i in ../SampleTests/test*; do
    [[ "$i" = *"out" ]] && continue;
    echo Running `basename $i`
    bash $i > $i.out
    echo Diff for `basename $i`:
    diff $i.out $i.expout
done
