#!/bin/bash

for i in AAA Base Cryptographic IcingaSenderModule TcpStandardModule
do
	cd ${i};
	mkdir src;
	mkdir include;

	git mv *.cpp ./src;
	git mv *.h ./include;
	cd ..;
done
