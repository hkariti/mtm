#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define BITS_IN_INT (sizeof(int)*8)
int main() {
    int numbers_count, exponents_sum, *numbers;
    int exponent, set_bits, temp;

    printf("Enter size of input:\n");
    if ((scanf("%d", &numbers_count) != 1) || (numbers_count <= 0)) {
        printf("Invalid size\n");
        return 1;
    }

    numbers = malloc(numbers_count*sizeof(int));
    printf("Enter numbers:\n");
    for (int i=0; i < numbers_count; i++) {
        if ((scanf("%d", &numbers[i]) != 1)) {
            printf("Invalid number\n");
            free(numbers);
            return 1;
        }
    }
    exponents_sum = 0;
    for (int i=0; i < numbers_count; i++) {
        set_bits = 0;
        exponent = 0;
        temp = numbers[i];
        if (temp < 0) {
            continue;
        }
        for (int j=0; j < BITS_IN_INT; j++) {
            if ((temp & 1) == 1) {
                set_bits++;
                exponent = j;
            }
            temp = temp >> 1;
        }
        if (set_bits == 1) {
            printf("The number %d is a power of 2: %d = 2^%d\n", numbers[i], numbers[i], exponent);
            exponents_sum += exponent;
        }
    }
    printf("Total exponent sum is %d\n", exponents_sum);

    free(numbers);
    return 0;
    
}
