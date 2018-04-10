#!/usr/bin/env bash

clear


file=controller.cc

for count in {1..20};
do
  WINDOW_SIZE_VAL=$(($count*10))
  echo "Test $count with window size $WINDOW_SIZE_VAL\n"
  WINDOW_SIZE=$WINDOW_SIZE_VAL ./run-contest fixed_window &>> varied_windown_results.txt;

done


