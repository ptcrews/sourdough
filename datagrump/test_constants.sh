#!/usr/bin/env bash

clear

DIR="tests/"
OUT_FILE_POSTFIX=test.out
rm $DIR*$OUT_FILE_POSTFIX

for inc in {0..5};
do
  for rtt_delta in {0..10};
  do
    RTT_DELTA_VAL=$(($rtt_delta + 10))
    INC_VAL=$((5 + 5*$inc))
    OUT_FILE="inc-$INC_VAL-rtt_delta-$RTT_DELTA_VAL-$OUT_FILE_POSTFIX"
    echo "Test params: $OUT_FILE" >> $DIR$OUT_FILE
    INC=$INC_VAL RTT_DELTA=$RTT_DELTA_VAL ./run-contest fixed_window >> $DIR$OUT_FILE 2>&1;
  done
done

for num in {1..7};
do
  OUT_FILE="test-$num-$OUT_FILE_POSTFIX"
  echo "Test number: $num" >> $DIR$OUT_FILE
  ./run-contest-trace-$num fixed_window >> $DIR$OUT_FILE 2>&1;
done
