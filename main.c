#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "hashmap.h"

#define KEY_PREFIX ("K_")
#define KEY_MAX_LENGTH (0xff)

/* Hashmap data structure */
typedef struct data_t {
  char key[KEY_MAX_LENGTH];
  int val;
} data_t;

/* Sort */
void insertion_sort(int * arr, int len) {
  int i, key, j;
  for (i = 1; i < len; i++) {
    key = arr[i];
    j = i - 1;
    while (j >= 0 && arr[j] > key) {
      arr[j + 1] = arr[j];
      j -= 1;
    }
    arr[j + 1] = key;
  }
}

/* Theta n^2 */
int exponential_way(int * arr, size_t arrlen, int k) {

  int computed_pairs = 0;
  for (size_t i = 0; i < arrlen; i++) {
    for (size_t j = i+1; j < arrlen; j++) {
      if (arr[i] + arr[j] == k)
	computed_pairs++;
    }
  }
  return computed_pairs;
}

/* Theta n */
int linear_way(int * arr, size_t arrlen, int k) {

  int error = -1;
  map_t map;
  char key_string[KEY_MAX_LENGTH];
  data_t * value;
  int twice_cnt = 0;
  
  map = hashmap_new();

  /* Populate unordered map with all values from input */
  for (size_t i = 0; i < arrlen; i++) {
    value = malloc(sizeof(data_t));
    snprintf(value->key, KEY_MAX_LENGTH, "%s%d", KEY_PREFIX, arr[i]);
    value->val = arr[i];

    error = hashmap_put(map, value->key, value);
    assert(error==MAP_OK);
  }
  
  /* OPTIONAL: Verify all keys are inserted correctly. */
  for (size_t i = 0; i < arrlen; i++) {
    snprintf(key_string, KEY_MAX_LENGTH, "%s%d", KEY_PREFIX, arr[(int)i]);
    error = hashmap_get(map, key_string, (void **)(&value));
    assert(error == MAP_OK);
    assert(value->val == arr[i]);
  }

  /* Do the actual counting */
  for (size_t i = 0; i < arrlen; i++) {

    snprintf(key_string, KEY_MAX_LENGTH, "%s%d", KEY_PREFIX, k-arr[i]);

    if ((MAP_OK == hashmap_get(map, key_string, (void **)(&value)))) {
      twice_cnt++;
    }

    if (k - arr[i] == arr[i])
      twice_cnt--;
  }
  
  return twice_cnt / 2;
}

/* Theta n log n */
int log_way(int * arr, size_t arrlen, int k) {

  int lo = 0, hi = arrlen -1;
  int counted_pairs = 0;
  
  /* First, we want to sort the array */
  insertion_sort(arr, arrlen);

  while (lo < hi) {
    if (arr[lo] + arr[hi] == k)
      counted_pairs++;
    (arr[lo] + arr[hi] < k ) ? lo++ : hi--;
  }
  return counted_pairs;
}

int * generate_array(size_t upper_limit) {

  int * arr = (int *) malloc(sizeof(int) * upper_limit);

  for (size_t i = 0; i < upper_limit; i++) {
    while (true) {
      int intermediate = (random() % 10000);
      bool distinct = true;
      for (size_t j = 0; j <= i; j++) {
	if (arr[j] == intermediate)
	  distinct = false;
      }
      if (distinct) {
	arr[i] = intermediate;
	break;
      }
    }
  }
  return arr;
}

void print_array(int * arr, size_t arrlen) {

  for (size_t i = 0; i < arrlen; i++) {
    printf("Index %zu: %d\t", i, arr[i]);
    if (i % 4 == 0)
      printf("\n");
  }
  printf("\n");
}

int main(int argc, char * argv[]) {

  size_t len;
  srandom(time(NULL));
  
  if (argc < 2)
    len = 1000;
  else
    len = (size_t) atoi(argv[1]);

  int * arr = generate_array(len);
  int k = (random() % 1000) * 2;

  /* print_array(arr, len); */
  
  printf("K value: %d\n", k);
  printf("Exponential way returned: %d\n", exponential_way(arr, len, k));
  printf("Logarithmic way returned: %d\n", log_way(arr, len, k));
  printf("Linear way returned: %d\n", linear_way(arr, len, k));
    
  if (arr != NULL)
    free(arr);
  
  return 0;
}
