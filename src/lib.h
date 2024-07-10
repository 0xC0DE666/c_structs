#include <stdbool.h>

int read_line(char buffer[], int size);
int string_length(char buffer[]);

typedef struct Element {
  int index;
  void* value;
} Element;

typedef struct Array {
  int size;
  int count;
  Element* elements;
} Array;

Array* array_new(int size);
char* array_to_string(Array *array);
int array_add(Array *array, void* value);
void* array_get(Array* array, int index);
void* array_remove(Array* array, int index);
