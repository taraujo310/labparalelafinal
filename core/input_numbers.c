#include "input_numbers.h"
#include <stdio.h>
#include <stdlib.h>

void get_numbers(int *numbers) {
    FILE *f = fopen("core/numbers.txt", "r");
    int num;
    int i = 0;

    while(fscanf(f, "%d", &num) > 0) {
        numbers[i++] = num;
    }

    fclose(f);
}

void print_numbers(int *numbers, int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", numbers[i]);
    }
}