#include "types.h"
#include "lists.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

//frees Variable Struct properly
void freeVariableStruct(Variable* var) {
  free(var->name);
  if(var->type==STRING) {
    free(var->data.str->string);
    free(var->data.str);
  } else if(var->type==ARRAY) {
    list_free(var->data.array);
  }
  free(var);
}

//return Variable struct with given parameters
//NOTES len_of_str_data only matters if type is STRING 
//for array, the List* struct must have already been created and allocated
Variable* createVariableStruct(TYPE type, char* variable_name, void* data, size_t len_of_str_data) {
  //mallocs
  Variable* node=malloc(sizeof(Variable));
  node->type=type;
  node->name=variable_name;

  if(type == STRING) {
    String* str=malloc(sizeof(String));
    str->string=(char*)data;
    str->length=len_of_str_data;
    node->data.str=str;
  } else if(type == INTEGER) {
    node->data.integer= *(int*) data;
  } else if(type == DOUBLE) {
    node->data.floatingpoint= *(double*) data;
  } else if(type == BOOL) {
    node->data.boolean = *(int*) data;
  } else if(type == ARRAY) {
    node->data.array=(List*)data;
  } else if(type == _NULL) {
    //NO OPERATION IS NEEDED
  } else if(type == VAR) {
    String* str1=malloc(sizeof(String));
    str1->string=(char*)data;
    str1->length=len_of_str_data;
    node->data.str=str1;
  } else if(type == FUNCTION) {
    //TODO
  } else if(type == ARITHMETIC_EXPRESSION) {
    //TODO
  } else if(type == BOOL_EXPRESSION) {
    //TODO
  }
  return node;
}



