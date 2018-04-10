#!/usr/bin/env bash

clear


file=controller.cc

for count in {1..20}
do
    WINDOW_SIZE={$COUNT*10} ./run-contest fixed_window 2>> varied_windown_results.txt

done


