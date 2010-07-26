#!/bin/bash


echo "copy the get_gif_width_height to test dir"
cd ./test
cp ../get_gif_width_height ./
echo "auto test"
./get_gif_width_height test

