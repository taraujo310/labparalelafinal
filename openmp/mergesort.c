#include "../core/input_numbers.h"
#include <stdio.h>
#include <omp.h>
#define SIZE 1024

void merge(int arr[], int left_index, int middle, int right_index) 
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

void mergesort(int numbers[], int left_index, int right_index) 
{ 
    if (left_index < right_index) 
    {
        int middle = left_index+(right_index-left_index)/2;

        #pragma omp parallel sections
        {
            #pragma omp section
            {
                mergesort(numbers, left_index, middle); 
            }
            #pragma omp section
            {
                mergesort(numbers, middle+1, right_index); 
            }
        }
  
        merge(numbers, left_index, middle, right_index); 
    } 
} 

int main() {
    int numbers[SIZE];

    get_numbers(numbers);

    mergesort(numbers, 0, SIZE-1);

    //print_numbers(numbers, SIZE);
    
    return 0;
}