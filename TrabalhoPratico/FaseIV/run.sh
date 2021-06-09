#!/bin/sh

build_dir=build
mkdir -p $build_dir
cmake -S . -B $build_dir > /dev/null 2>&1
cd $build_dir || exit 1
make > /dev/null 2>&1
./generator sphere 5 20 20 ../models/sphere.3d 
./generator torus 5 1 20 20 ../models/torus.3d
./generator patch 2 ../models/teapot.patch ../models/teapot.3d
./engine ../scenes/solarsystem.xml
