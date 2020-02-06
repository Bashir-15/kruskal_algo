#!/bin/bash

INDEX=2

#$ -cwd
#$ -l h_rt=00:01:00

while [ $INDEX -lt 2048 ]; do
	#$ -pe ompi $INDEX
	mpirun q2 $ARG1
	echo $ARG1
	$INDEX=$((INDEX*2))
done
