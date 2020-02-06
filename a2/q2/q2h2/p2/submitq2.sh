#!/bin/bash
#$ -cwd
#$ -l h_rt=00:01:00
#$ -pe  ompi 4
time mpirun q2 0 5 0.0000001
