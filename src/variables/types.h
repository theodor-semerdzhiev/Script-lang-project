#ifndef TYPES_H
#define TYPES_H

#include <stddef.h>

typedef enum{
  INTEGER,
  DOUBLE,
  STRING,
  BOOL,
  ARRAY,
  FUNCTION,
  VAR,
  _NULL,
  UNKNOWN
} TYPE;

typedef struct {
  char* string;
  int length;
} String;

//TODO to be implemented
typedef struct {
  TYPE type;
  int length;

} Array;

union data_types{
  int integer;
  double floatingpoint;
  String *str;
  int boolean;
  //arrays to be added TODO
};

typedef struct {
  TYPE type;
  char* name;
  union data_types data;
} Variable;

void freeStringStruct(String *str);
void freeVariableStruct(Variable *var);
Variable* createVariableStruct(TYPE type, char* variable_name, void* data, size_t len_of_str_data);
void modifyVariable(Variable *varStruct, char* variable_name, void* data, TYPE type_of_var);
#endif 
