#!/bin/bash

THREADS=( 1 8 16 )
ARGS="-1.8 -1.8 1.8 1.8 6000 6000 0"

for threads in "${THREADS[@]}"
do
    echo ""
    echo "=> Running with $threads threads"

    for (( i=0; i<20; i++ ))
    do
        filename="tests/${threads}-$i.txt"
        truncate -s 0 "$filename"

        echo "Test $i"
        if [ $threads -eq 1 ]
        then
            /usr/bin/time -o $filename ./mbrot_seq $ARGS $threads "/dev/null"
        else
            /usr/bin/time -o $filename ./mbrot $ARGS $threads "/dev/null"
        fi
    done

done
