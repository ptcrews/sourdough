#!/usr/bin/env bash

sed -n -e '
    /Test with /p;
    /signal delay: /s/.*: \([0-9]*\).*/\1/p;
    /throughput: /s/.*: \([0-9\.]*\).*/\1/p;
' varied_constants_results.txt 

