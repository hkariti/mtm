#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

typedef void* Element;
typedef int(*elementCompare)(Element, Element);

void mergeParts(Element *array, Element *first_part, Element *second_part,
                int first_length, int second_length,
                elementCompare compare_function) {
  int first_index, second_index, target_index;
  int total_length = first_length + second_length;
  // Merge the two parts into the target array
  for (first_index = 0, second_index = 0, target_index = 0;
       first_index < first_length && second_index < second_length;
       target_index++) {
    Element first_element = first_part[first_index];
    Element second_element = second_part[second_index];
    int cmp = compare_function(first_element, second_element);
    if (cmp <= 0) {
      array[target_index] = first_element;
      first_index++;
    } else {
      array[target_index] = second_element;
      second_index++;
    }
  }
  // Put the remainder of the two parts in what's left of the target array
  Element *remainder;
  if ( first_index < first_length) {
    remainder = &first_part[first_index];
  } else {
    remainder = &second_part[second_index];
  }
  for (int remainder_index = 0; target_index < total_length;
       target_index++, remainder_index++) {
    array[target_index]= remainder[remainder_index];
  }
}
void mergeSort(Element *array, int length, elementCompare compare_function) {
  assert(array && length >= 0);

  if (length <= 1) return;
  Element *array_copy = malloc(sizeof(Element) * length);
  assert(array_copy);
  for (int i = 0; i < length; i++) {
    array_copy[i]= array[i];
  }
  Element *first_part, *second_part;
  int first_length = length / 2;
  int second_length = length - first_length;
  first_part = array_copy;
  second_part = &array_copy[first_length];
  mergeSort(first_part, first_length, compare_function);
  mergeSort(second_part, length - first_length, compare_function);
  mergeParts(array, first_part, second_part, first_length, second_length,
             compare_function);
  free(array_copy);
}

int intCompare(int* a, int* b) {
  return *a - *b;
}

int main() {
  int length = 10;
  int** array = malloc(sizeof(int*)*length);

  // Init
  for (int i = 0; i < length; i++) {
    array[i] = malloc(sizeof(int));
    *(array[i]) = length - i;
  };

  for (int i = 0; i < length; i++) {
    printf("%d ", *(array[i]));
  }
  printf("\n");
  mergeSort(array, length, intCompare);

  for (int i = 0; i < length; i++) {
    printf("%d ", *(array[i]));
  }
  printf("\n");
  return 0;
}
