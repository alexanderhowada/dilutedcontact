#!/bin/bash

set -o verbose

array=( 005 020 050 085 100 )
for initProb in "${array[@]}"
do
	filename="run-timeseriesFull$initProb.cpp"
	cp $filename run-timeseriesFull.cpp
	make timeseriesFull
#	mpirun -n 27 --hostfile nos-bcdc.txt ./../2DDC-timeseriesFull DCP-timeseriesFull$initProb.db DilutedContact Inputspy.txt &> run-timeseriesFull.txt
	mpirun -n 5 ./../2DDC-timeseriesFull DCP-timeseriesFull$initProb.db DilutedContact Inputspy.txt &> run-timeseriesFull$initProb.txt
done
