#ifndef TYPES_H
#define TYPES_H

#include <stddef.h>

typedef struct List List;

typedef enum {
  INTEGER,
  DOUBLE,
  STRING,
  BOOL,
  ARRAY,
  FUNCTION,
  VAR,
  _NULL,
  ARITHMETIC_EXPRESSION,
  BOOL_EXPRESSION,
  UNKNOWN
} TYPE;

typedef struct {
  char* string;
  int length;
} String;

union data_types {
  int integer;
  double floatingpoint;
  String *str;
  int boolean;
  List *array;
};

typedef struct {
  TYPE type;
  char* name;
  union data_types data;
} Variable;

typedef struct List {
  int length;
  int Capacity;
  Variable** list;
} List;

void freeVariableStruct(Variable *var);
Variable* createVariableStruct(TYPE type, char* variable_name, void* data, size_t len_of_str_data);

#endif 