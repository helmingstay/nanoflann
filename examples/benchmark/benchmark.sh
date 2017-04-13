#!/bin/bash
run_name='2D'
OUTDIR="./results"
COND_FILE="$OUTDIR/conditions.$run_name.csv"
TIME_FILE="$OUTDIR/timings.$run_name.csv"

time_cmd=/usr/bin/time
            
echo "npoint,radius,nsearch" > $COND_FILE
echo "user,sys" > $TIME_FILE

make

iloop=0
for isearch in {2..4} 
    do
    nsearch=$((10**$isearch))
    ##
    for ipoint in {3..5}
    do
        npoint=$((10**$ipoint))
        ## search over unit box
        rads=(0.5 1 1.5 2)
        for rad in ${rads[@]}
        do
            ((iloop++))
            ## run the test, output conditions and times to separate files
            ## show progress
            echo -n "Test $iloop: "
            echo "$npoint,$rad,$nsearch" >> $COND_FILE
            $time_cmd -f "%U,%S" -a -o $TIME_FILE ./run_test $npoint $rad $nsearch >/dev/null
            tail -n 1 $COND_FILE
            tail -n 1 $TIME_FILE
        done
    done
done
