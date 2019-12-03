#include "../core/input_numbers.h"
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#define SIZE 1024

void bubblesort(int numbers[], int n)
{
    int i, j, temp;

    for (i = n-1; i > 0; i--){
        for (j = 0; j < i; j++) {
            if (numbers[j] > numbers[j+1]) {
                temp = numbers[j];
                numbers[j] = numbers[j + 1];
                numbers[j+1] = temp;
            }
        }
    }
}

void *merge(int v1[], int n1, int v2[], int n2) {
    int total_size = n1 + n2;
    int i = 0, j = 0, k;
    int *result = (int *) malloc((total_size)*sizeof(int));

    for (int k = 0; k < total_size; k++) {
        if (i >= n1)
            result[k] = v2[j++];
        else if (j >= n2)
            result[k] = v1[i++];
        else if (v1[i] < v2[j])
            result[k] = v1[i++];
        else
            result[k] = v2[j++];
    }

    return result;
}

int main(int argc, char **argv) {
    int *numbers = (int *)malloc(SIZE*sizeof(int));
    int *chunk;
    int *other;
    int processes_size, part_size, process_id, step, i, received_size, m;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_id);
    MPI_Comm_size(MPI_COMM_WORLD, &processes_size);

    part_size = SIZE/processes_size;

    if (process_id == 0) {
        get_numbers(numbers);

        MPI_Bcast(&part_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
        chunk = (int *)malloc(part_size*sizeof(int));
        MPI_Scatter(numbers, part_size, MPI_INT, chunk, part_size, MPI_INT, 0, MPI_COMM_WORLD);

        bubblesort(chunk, part_size);
    } else {
        MPI_Bcast(&part_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
        chunk = (int *)malloc(part_size*sizeof(int));
        MPI_Scatter(numbers, part_size, MPI_INT, chunk, part_size, MPI_INT, 0, MPI_COMM_WORLD);
        free(numbers);

        bubblesort(chunk, part_size);
    }
    
    step = 1;
	while(step < processes_size) {
		if(process_id % (2*step) == 0) {
			if(process_id + step < processes_size) {
				MPI_Recv(&m, 1, MPI_INT, process_id+step, 0, MPI_COMM_WORLD, &status);
				other = (int *)malloc(m*sizeof(int));

				MPI_Recv(other, m, MPI_INT, process_id+step, 0, MPI_COMM_WORLD, &status);
				chunk = merge(chunk, part_size, other, m);
				part_size = part_size + m;
			}
		} else {
			int near = process_id - step;
			MPI_Send(&part_size, 1, MPI_INT, near, 0, MPI_COMM_WORLD);
			MPI_Send(chunk, part_size, MPI_INT, near, 0, MPI_COMM_WORLD);
			break;
		}

		step = step*2;
	}

    if (process_id == 0) {
        //print_numbers(chunk, part_size);
        free(chunk);
        free(numbers);
    }

    MPI_Finalize();
    return 0;
}