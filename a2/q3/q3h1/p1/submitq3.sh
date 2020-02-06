#!/bin/bash
#$ -cwd
#$ -l h_rt=0:5:0
#$ -pe  ompi 4
time mpirun q3 0 5 0.000001
