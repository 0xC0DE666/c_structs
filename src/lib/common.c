#include <stdlib.h>
#include <stdbool.h>

#include "./c_structs.h"

Error* error(int code, char* message) {
  Error* err = malloc(sizeof(Error));
  err->code = code;
  err->message = message;
  return err;
}

void error_free(Error** error) {
  free(*error);
  *error = NULL;
}

Result success(void* value) {
  return (Result) { value, NULL };
}

Result fail(int code, char* message) {
  return (Result) { NULL, error(code, message) };
}
