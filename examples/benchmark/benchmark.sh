#!/bin/bash
runtime=$(date "+%F_%T")
OUTDIR="./results"
COND_FILE="$OUTDIR/conditions.$runtime.csv"
TIME_FILE="$OUTDIR/timings.$runtime.csv"

time_cmd=/usr/bin/time
            
echo "iloop,npoint,radius,nsearch" > $COND_FILE
echo "user,sys,elapsed" > $TIME_FILE

make

iloop=0
for isearch in {2..4} 
    do
    nsearch=$((10**$isearch))
    ##
    for ipoint in {3..6}
    do
        npoint=$((10**$ipoint))
        ##
        for rad in {0..10..2} 
        do
            ((iloop++))
            ## run the test, output conditions and times to separate files
            ## show progress
            echo "$iloop,$npoint,$rad,$nsearch" >> $COND_FILE
            $time_cmd -f "%U,%S,%E" -a -o $TIME_FILE ./run_test $npoint $rad $nsearch >/dev/null
            tail -n 1 $COND_FILE
            tail -n 1 $TIME_FILE
        done
    done
done
