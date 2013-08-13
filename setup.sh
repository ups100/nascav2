#!/bin/sh

if [ ! -d ./build ] 
then 
	mkdir build
	mkdir build/lib
	mkdir build/bin 
fi

cd build
cmake ../code
