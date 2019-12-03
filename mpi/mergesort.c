#include "../core/input_numbers.h"
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#define SIZE 1024

void _merge(int arr[], int left_index, int middle, int right_index);

void mergesort(int *numbers, int left_index, int right_index) 
{ 
    int *c;
	int mid = (left_index+right_index)/2;
	int lowerCount = mid - left_index + 1;
	int upperCount = right_index - mid;

	if (right_index == left_index) {
		return;
	} else {
		mergesort(numbers, left_index, mid);
		mergesort(numbers, mid+1, right_index);
		_merge(numbers, left_index, mid, right_index);
	}
}

void _merge(int arr[], int left_index, int middle, int right_index)
{ 
    int i, j, k, 
    size1 = middle - left_index + 1, 
    size2 =  right_index - middle;

    int left[size1], right[size2];

    for (i = 0; i < size1; i++) 
        left[i] = arr[left_index + i];

    for (j = 0; j < size2; j++) 
        right[j] = arr[middle + 1 + j]; 

    i = 0, j = 0, k = left_index;

    while (i < size1 && j < size2) {
        if (left[i] <= right[j]) {
            arr[k++] = left[i++];
        } else {
            arr[k++] = right[j++];
        }
    }

    while (i < size1)
        arr[k++] = left[i++];

    while (j < size2)
        arr[k++] = right[j++];
}

int *merge(int *v1, int n1, int *v2, int n2) {
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

        mergesort(chunk, 0, part_size-1);
        //print_numbers(chunk, part_size);
    } else {
        MPI_Bcast(&part_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
        chunk = (int *)malloc(part_size*sizeof(int));
        MPI_Scatter(numbers, part_size, MPI_INT, chunk, part_size, MPI_INT, 0, MPI_COMM_WORLD);

        mergesort(chunk, 0, part_size-1);
        //print_numbers(chunk, part_size);
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