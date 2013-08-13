#!/bin/sh

if [ ! -d ./build ] 
then 
	mkdir build
	mkdir build/lib
	mkdir build/bin 
fi

cd build
cmake -DQt5Core_DIR=/home/kopasiak/Qt/5.1.0/gcc_64/lib/cmake/Qt5Core/ ../code
