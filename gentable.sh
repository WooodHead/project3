#!/bin/bash
X=2
while [ $X -lt 23 ]
do
    ./project2 $X 
    let X=X+1
done
