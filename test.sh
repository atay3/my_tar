#!/usr/bin/env bash

f1="/home/ashtay/my_tar/test/ref1.tar"
f2="/home/ashtay/my_tar/test/test1.tar"
f3="include/utils.h"

rm -f $f1 $f2

tar cf $f1 $1
./my_tar -cf $f2 $1

tar rf $f1 $f3
./my_tar -rf $f2 $f3

diff <(xxd $f1) <(xxd $f2) -ys
wc -c $f1 $f2