#include "types.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

void freeVariableStruct(Variable* var);
Variable* createVariableStruct(TYPE type, char* variable_name, void* data, size_t len_of_str_data);
void modifyVariable(Variable *varStruct, char* variable_name, void* data, TYPE type_of_var);

//frees Variable Struct properly
void freeVariableStruct(Variable* var) {
  free(var->name);
  if(var->type==STRING) {
    free(var->data.str->string);
    free(var->data.str);
  }
  free(var);
}

//return Variable struct with given parameters
//NOTES len_of_str_data only matters if type is STRING (or Array, but this is not implemented yet...)
Variable* createVariableStruct(TYPE type, char* variable_name, void* data, size_t len_of_str_data) {
  //mallocs
  Variable* node=malloc(sizeof(Variable));
  node->type=type;
  node->name=variable_name;
  switch(type) {
    case STRING:
      String* str=malloc(sizeof(String));
      str->string=(char*)data;
      str->length=len_of_str_data;
      node->data.str=str;
      break;
    case INTEGER:
      node->data.integer= *(int*) data;
      break;
    case DOUBLE:
      node->data.floatingpoint= *(double*) data;
      break;
    case BOOL:
      node->data.boolean = *(int*) data;
      break;
    case ARRAY:
      //TODO
      break; 
    case _NULL:
      break;
    default:
      break;
  }
  return node;
}
//modifies Variable accordingly
void modifyVariable(Variable *varStruct, char* variable_name, void* data, TYPE type_of_var) {
  //changes the name and TYPE enum
  free(varStruct->name);
  varStruct->name=variable_name;
  switch(type_of_var) {
    case STRING:
      //if the struct is already a string, then we can just modify the exsisting struct
      if(varStruct->type ==STRING) {
        free(varStruct->data.str->string);
        varStruct->data.str->string=(char*) data;
        varStruct->data.str->length=strlen((char*) data);
        
      //otherwise we create a new string struct
      } else {
        String *newStr=malloc(sizeof(newStr));
        newStr->length=strlen(variable_name);
        newStr->string=(char*)data;
        varStruct->data.str=newStr;
      }
      break;
    case ARRAY:
      //TODO
      break;
    case INTEGER:
      varStruct->data.integer= *(int*) data;
      break;
    case DOUBLE:
      varStruct->data.floatingpoint= *(double*) data;
      break;
    case BOOL:
      varStruct->data.boolean= *(int*) data;
      break; 
    default:
      break;
  }
  varStruct->type=type_of_var;
}


