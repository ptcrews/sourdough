#!/usr/bin/env bash

clear

OUT_FILE=varied_constants_results.txt
rm $OUT_FILE

for count in {0..6};
do
    TEMP=$(($count-3))
    TEMP2="$(echo "$TEMP * 0.04" | bc -l)" 
    INC_CONST_VAL="$(echo "$TEMP2 + 0.5" | bc -l)"
  for count2 in {1..6};
  do   

      TEMP3=$(($count2-3))
      TEMP4="$(echo "$TEMP3 * 0.004" | bc -l)" 
      DEC_CONST_VAL="$(echo "$TEMP4 + 0.013" | bc -l)"
      echo "Test with increase constant $INC_CONST_VAL and decrease constant $DEC_CONST_VAL" >> $OUT_FILE
      INC_CONST=$INC_CONST_VAL DEC_CONST=$DEC_CONST_VAL ./run-contest hud >> $OUT_FILE 2>&1;
  done
done


