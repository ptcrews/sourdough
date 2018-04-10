#!/usr/bin/env bash

clear


file=controller.cc

window_size=1
for count in {1..20}
do
window_size = count*5
sed -i 's/the_window_size = *;/the_window_size = '$window_size';/' $file
#./run-contest fixed_window 2>> fixed_windown_repeat.txt

done


