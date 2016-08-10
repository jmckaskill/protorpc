#!/bin/sh
PATH="$PATH:/c/c"
for f in *.bin; do
    echo $f
    cat $f | ncat localhost 8080
done