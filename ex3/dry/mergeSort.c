#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

// Type of a generic element
typedef void* Element;

/* Function that compares two elements, used for sorting.
 *
 * @return
 *  Positive number - if element1 > element2
 *  Zero            - if element1 = element2
 *  Negative number - if element1 < element2
 */
typedef int(*elementCompare)(Element element1, Element element2);

/**
 * mergeParts - Create a sorted array from 2 sorted arrays
 *
 * @param array         - Target array of generic Elements, should be of size
 *                        first_length + second_length.
 * @param first_part    - First array. Must be sorted according to
 *                        compare_function.
 * @param second_part   - Second array. Must be sorted according to
 *                        compare_function.
 * @param first_length  - Length of first_part.
 * @param second_length     - Length of second_part.
 * @param compare_function  - Fuction that compares elements in first_part
 *                            and second_part. See docs for elementCompare.
 */
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
  if (first_index < first_length) {
    remainder = &first_part[first_index];
  } else {
    remainder = &second_part[second_index];
  }
  for (int remainder_index = 0; target_index < total_length;
       target_index++, remainder_index++) {
    array[target_index]= remainder[remainder_index];
  }
}

/**
 * mergeSort - Sorts an array of generic elements using compare_function
 *
 * @param array             - The array to sort.
 * @param length            - Length of array.
 * @param compare_function  - Function to compare two elements. See docs for
 *                            elementCompare.
 */
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
