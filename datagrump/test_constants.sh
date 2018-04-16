#!/usr/bin/env bash

clear

DIR="tests/"
OUT_FILE_POSTFIX=test.out
rm $DIR*$OUT_FILE_POSTFIX

for seq_timeout in {2..4};
do
  OUT_FILE="seq_timeout-$SEQ_TIMEOUT_VAL-$OUT_FILE_POSTFIX"
  echo "Test params: $OUT_FILE" >> $DIR$OUT_FILE
  SEQ_TIMEOUT=$SEQ_TIMEOUT_VAL ./run-contest fixed_window >> $DIR$OUT_FILE 2>&1;
done

for alpha in {1..5};
do
  OUT_FILE="alpha-$alpha-OUT_FILE_POSTFIX"
  echo "Test params: $OUT_FILE" >> $DIR$OUT_FILE
  ALPHA=$alpha ./run-contest fixed_window >> $DIR$OUT_FILE 2>&1;
done

for dec in {1..5};
do
  OUT_FILE="dec-$dec-OUT_FILE_POSTFIX"
  echo "Test params: $OUT_FILE" >> $DIR$OUT_FILE
  DEC=$dec ./run-contest fixed_window >> $DIR$OUT_FILE 2>&1;
done

for inc in {1..5};
do
  OUT_FILE="inc-$inc-OUT_FILE_POSTFIX"
  echo "Test params: $OUT_FILE" >> $DIR$OUT_FILE
  INC=$inc ./run-contest fixed_window >> $DIR$OUT_FILE 2>&1;
done

for timeout in {2..4};
do
  OUT_FILE="timeout-$timeout-OUT_FILE_POSTFIX"
  echo "Test params: $OUT_FILE" >> $DIR$OUT_FILE
  TIMEOUT=$timeout ./run-contest fixed_window >> $DIR$OUT_FILE 2>&1;
done

for rtt_delta in {0..4};
do
  RTT_DELTA_VAL=$(($rtt_delta*5 + 10))
  OUT_FILE="$RTT_DELTA_VAL-$OUT_FILE_POSTFIX"
  echo "Test params: $OUT_FILE" >> $DIR$OUT_FILE
  RTT_DELTA=$RTT_DELTA_VAL ./run-contest fixed_window >> $DIR$OUT_FILE 2>&1;
done
