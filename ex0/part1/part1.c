#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define BITS_IN_INT (sizeof(int)*8)

void sum_exponents(int, int*);

int main() {
    int numbers_count, *numbers;

    printf("Enter size of input:\n");
    if ((scanf("%d", &numbers_count) != 1) || (numbers_count <= 0)) {
        printf("Invalid size\n");
        return 0;
    }

    numbers = malloc(numbers_count*sizeof(int));
    printf("Enter numbers:\n");
    for (int i=0; i < numbers_count; i++) {
        if ((scanf("%d", &numbers[i]) != 1)) {
            printf("Invalid number\n");
            free(numbers);
            return 0;
        }
    }

    sum_exponents(numbers_count, numbers);

    free(numbers);
    return 0;
}

void sum_exponents(int numbers_count, int *numbers) {
    int exponent, exponents_sum, set_bits, current_number;

    exponents_sum = 0;
    for (int i=0; i < numbers_count; i++) {
        set_bits = 0;
        exponent = 0;
        current_number = numbers[i];
        if (current_number < 0) {
            continue;
        }
        // Count the number of set bits in the current number
        // Also save the index of the left-est bit set
        for (int j=0; j < BITS_IN_INT; j++) {
            if ((current_number & 1) == 1) {
                set_bits++;
                exponent = j;
            }
            current_number = current_number >> 1;
        }
        // Numbers that are a power of two have only 1 bit set
        if (set_bits == 1) {
            printf("The number %d is a power of 2: %d = 2^%d\n", numbers[i], numbers[i], exponent);
            exponents_sum += exponent;
        }
    }
    printf("Total exponent sum is %d\n", exponents_sum);
}
