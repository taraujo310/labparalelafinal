#!/bin/bash

echo Compilando...

mpicc mpi/bubblesort.c core/input_numbers.c -o bubblesort_mpi.out
mpicc mpi/mergesort.c core/input_numbers.c -o mergesort_mpi.out
gcc -o mergesort_openmp.out openmp/mergesort.c core/input_numbers.c -fopenmp
gcc -o bubblesort_openmp.out openmp/bubblesort.c core/input_numbers.c -fopenmp

set OMP_NUM_THREADS=4

echo Starting...

echo ======= Mergesort MPI =======

for i in 1 2 3 4 5 6 7 8 9 10; do
    /usr/bin/time -f "%e" mpirun -np 4 mergesort_mpi.out
done

echo ======= Mergesort OPENMP =======

for i in 1 2 3 4 5 6 7 8 9 10; do
    /usr/bin/time -f "%e" ./mergesort_openmp.out
done

echo ======= Bubblesort MPI =======

for i in 1 2 3 4 5 6 7 8 9 10; do
    /usr/bin/time -f "%e" mpirun -np 4 bubblesort_mpi.out
done

echo ======= Bubblesort OPENMP =======

for i in 1 2 3 4 5 6 7 8 9 10; do
    /usr/bin/time -f "%e" ./bubblesort_openmp.out
done