#!/bin/bash


echo "genearse the libfunlog.a"
make clean;make

echo "genearse the test bin"
cd ./src/test
make clean;make
