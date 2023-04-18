#include "types.h"
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
    //TODO
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
//modifies Variable accordingly
void modifyVariable(Variable *varStruct, char* variable_name, void* data, TYPE type_of_var) {
  //changes the name and TYPE enum
  free(varStruct->name);
  varStruct->name=variable_name;

  //frees string if we are not changing the type to a string 
  if(varStruct->type == STRING && type_of_var != STRING) {
    free(varStruct->data.str->string);
    free(varStruct->data.str);
  }
  if(type_of_var == STRING) {
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
  } else if(type_of_var == ARRAY) {
    //TODO
    
  } else if(type_of_var == INTEGER) {
    varStruct->data.integer= *(int*) data;
    
  } else if(type_of_var == DOUBLE) {
    varStruct->data.floatingpoint= *(double*) data;
    
  } else if(type_of_var == BOOL) {
    varStruct->data.boolean= *(int*) data;
    
  } else if(type_of_var == _NULL) {
    //NO OPERATION IS NEEDED
  } else if(type_of_var == FUNCTION) {
    //TODO
  } else if(type_of_var == ARITHMETIC_EXPRESSION) {
    //TODO
  } else if(type_of_var == BOOL_EXPRESSION) { 
    //TODO
  }
    
  varStruct->type=type_of_var;
}



