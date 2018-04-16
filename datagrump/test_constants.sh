#!/usr/bin/env bash

clear

DIR="tests/"
OUT_FILE_POSTFIX=test.out
rm $DIR*$OUT_FILE_POSTFIX

for seq_timeout in {2..4};
do
  for alpha in {1..5};
  do
    for dec in {1..5};
    do
      for inc in {1..5};
      do
        for timeout in {2..4};
        do
          for rtt_delta in {0..4};
          do
            SEQ_TIMEOUT_VAL=$(($seq_timeout*100))
            RTT_DELTA_VAL=$(($rtt_delta*5 + 10))
            OUT_FILE="$SEQ_TIMEOUT_VAL-$alpha-$dec-$inc-$timeout-$RTT_DELTA_VAL-$OUT_FILE_POSTFIX"
            echo "Test params: $OUT_FILE" >> $DIR$OUT_FILE
            SEQ_TIMEOUT=$SEQ_TIMEOUT_VAL ALPHA=$alpha DEC=$dec INC=$inc TIMEOUT=$timeout RTT_DELTA=$RTT_DELTA_VAL ./run-contest fixed_window >> $DIR$OUT_FILE 2>&1;
          done
        done
      done
    done
  done
done


