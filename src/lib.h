#include <stdbool.h>

int read_line(char buffer[], int capacity);
int string_length(char buffer[]);

typedef struct Element {
  int index;
  void *value;
} Element;

typedef struct Array {
  int capacity;
  int length;
  Element **elements;
} Array;

Array *array_new(int capacity);
char *array_to_string(Array *array);
int array_add(Array *array, void *value);
void *array_get(Array *array, int index);
void *array_remove(Array *array, int index);
