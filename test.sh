#!/usr/bin/env bash

f1="/home/ashtay/my_tar/test/ref2.tar"
f2="/home/ashtay/my_tar/test/test2.tar"

rm -f $f1 $f2

tar cf $f1 $1 $2
./my_tar -cf $f2 $1 $2

diff <(xxd $f1) <(xxd $f2) -ys
wc -c $f1 $f2