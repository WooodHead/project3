#!/bin/bash
X=5
while [ $X -lt 7 ]
do
    ./project3 $X > test.log
    cat test.log
    python test.py -i test.log
    echo " "
    rm test.log
    let X=X+1
done
