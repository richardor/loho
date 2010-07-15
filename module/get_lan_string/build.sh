#!/bin/bash

echo "cd the test dir"
cd ./test
make clean;make

echo "now auto test"
echo "wait..."
./get_string_test

