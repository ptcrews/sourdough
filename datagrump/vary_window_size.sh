#!/usr/bin/env bash

clear

OUT_FILE=varied_windown_results.txt
rm $OUT_FILE

for count in {1..20};
do
  WINDOW_SIZE_VAL=$(($count*10))
  echo "Test $count with window size $WINDOW_SIZE_VAL" >> $OUT_FILE
  WINDOW_SIZE=$WINDOW_SIZE_VAL ./run-contest fixed_window >> $OUT_FILE 2>&1;

done


