#!/bin/bash

./configure --prefix=/home/extend/install/ffmpeg-0.5.3 \
			--enable-gpl \
			--enable-shared \
			--enable-static \
			--cross-prefix=arm-hismall-linux- \
			--enable-cross-compile \
			--arch=armv5 \
			--disable-stripping \
			--target-os=linux  \
			--disable-network \
			--disable-ipv6 \
			--disable-aandct \
			--disable-mdct \
			--disable-rdft \
			--disable-encoders \
			--disable-indevs \
			--disable-outdevs \
			--disable-devices \
			--disable-filters \
			--disable-decoders \
			--enable-decoder=h264 \
			--disable-parsers \
			--disable-bsfs \
			--disable-muxers \
			--disable-ffmpeg \
			--disable-ffplay \
			--disable-ffserver \
			--disable-demuxers \
			--enable-demuxer=avi \
			--enable-small

