#include <stdbool.h>

int read_line(char buffer[], int size);
int string_length(char buffer[]);

typedef struct Array {
  int size;
  int count;
  int *values;
} Array;

Array *array_new(int size);
char* array_to_str(Array *array);
int array_add(Array *array, int value);
