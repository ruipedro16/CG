#!/bin/sh
build_dir=build
mkdir -p $build_dir
cmake -S . -B $build_dir
cd $build_dir || exit 1
make
