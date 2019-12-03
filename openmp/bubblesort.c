#include "../core/input_numbers.h"
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define SIZE 1024

void bubblesort(int numbers[], int n) {
    int change = 1, start = 0, i, temp;

    while(change || start) {
        change = 0;

        #pragma omp parallel for private(temp)
        for (int i = start; i < n-1; i += 2) {
            if (numbers[i] > numbers[i + 1]) {
                temp = numbers[i];
                numbers[i] = numbers[i + 1];
                numbers[i+1] = temp;
                change = 1;
            }
        }

        (start == 0) ? (start = 1) : (start = 0);
    }
}

int main() {
    int numbers[SIZE];

    get_numbers(numbers);

    bubblesort(numbers, SIZE);

    //print_numbers(numbers, SIZE);
    
    return 0;
}