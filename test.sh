#!/usr/bin/env bash

f1="./file.tar"
f2="./file_copy.tar"
f3="README.md"

rm -f $f1 $f2

tar cf $f1 $1
./my_tar -cf $f2 $1

tar rf $f1 $f3
./my_tar -rf $f2 $f3

diff <(xxd $f1) <(xxd $f2) -ys
wc -c $f1 $f2
