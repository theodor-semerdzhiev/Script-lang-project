#include "types.h"
#include "../bool.h"
#include "lists.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static int areVariableTypesValidForArithmeticComputation(Variable* var1, Variable* var2);

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
  
  //this if chain checks the type of the variable we want to create
  //we then properly set the data field in the Variable struct
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

//this variable takes a pair of variables and determines if those are valid for computing arithmetic operation
static int areVariableTypesValidForArithmeticComputation(Variable* var1, Variable* var2) {
  if(var1->type == UNKNOWN || var2->type == UNKNOWN){
    //THROW ERROR
    return 0;
  }
  if(var1->type == _NULL || var2->type == _NULL){
    //THROW ERROR
    return 0;
  }
  if(var1->type == ARRAY || var2->type == ARRAY) {
    //THROW ERROR
    return 0;
  }
  if(var1->type == FUNCTION || var1-> type == FUNCTION) {
    //THROW ERROR
    return 0;
  }
  //strings can represent numerical values, only if they are chars (strlen() == 1)
  if( var1->type== STRING && var1->data.str->length > 1 ||
      var2->type == STRING && var2->data.str->length > 1) {
    //THROW ERROR
    return 0;
  }

  return 1;
}

//this function takes 2 variables struct and creates a new variable struct, representing the sum of both variables
//NOTE: the returned struct's name field is NULL
Variable* AddVariables(Variable* var1, Variable *var2) {
  if(areVariableTypesValidForArithmeticComputation(var1,var2) == 0) 
    return NULL;

  //mallocs resulting
  Variable* result_of_addition=malloc(sizeof(Variable));
  result_of_addition->name=NULL;

  //if both variables are doubles
  if(var1->type == DOUBLE && var2->type == DOUBLE) {
    result_of_addition->type=DOUBLE;
    result_of_addition->data.floatingpoint=var1->data.floatingpoint + var2->data.floatingpoint;

  } else if(var1->type == DOUBLE) {
    int valueOfvar2=0;
    //if its a string then we know its a char (strlen == 1)
    if(var2->type == STRING) 
      valueOfvar2= (int) var1->data.str->string[0];
    else if(var2->type == INTEGER) 
      valueOfvar2=var1->data.integer;
    else if(var2->type == BOOL) 
      valueOfvar2=var1->data.boolean;
    
    //performs computation
    result_of_addition->type=DOUBLE;
    result_of_addition->data.floatingpoint=var1->data.floatingpoint + (double) valueOfvar2;

  } else if(var2->type == DOUBLE) {
    int valueOfvar1=0;
    //if its a string then we know its a char (strlen == 1)
    if(var2->type == STRING) 
      valueOfvar1= (int) var1->data.str->string[0];
    else if(var2->type == INTEGER) 
      valueOfvar1=var1->data.integer;
    else if(var2->type == BOOL) 
      valueOfvar1=var1->data.boolean;
    
    //performs computation
    result_of_addition->type=DOUBLE;
    result_of_addition->data.floatingpoint=valueOfvar1+result_of_addition->data.floatingpoint;
  } else {
    int valueOfvar1=0;
    int valueOfvar2=0;

    //these if statements sets the integer values of the integer fields
    if(var2->type == STRING) 
      valueOfvar2=(int) var1->data.str->string[0];
    else if(var2->type == INTEGER) 
      valueOfvar2=var1->data.integer;
    else if(var2->type == BOOL) 
      valueOfvar2=var1->data.boolean;

    if(var1->type == STRING) 
      valueOfvar1=(int) var1->data.str->string[0];
    else if(var1->type == INTEGER) 
      valueOfvar1=var1->data.integer;
    else if(var1->type == BOOL) 
      valueOfvar1=var1->data.boolean;

    result_of_addition->type=INTEGER;
    //computes the addition
    result_of_addition->data.integer= valueOfvar2 + valueOfvar1;
  }

  return result_of_addition;
}

//this function takes 2 variable structs as parameter and returns a malloced variable 
//representing var1 - var2
//NOTE: the returned struct's name field is NULL
Variable* SubstractVariables(Variable* var1, Variable *var2) {
  if(areVariableTypesValidForArithmeticComputation(var1,var2) == 0) 
    return NULL;

  //mallocs resulting
  Variable* result_of_addition=malloc(sizeof(Variable));
  result_of_addition->name=NULL;

  //if both variables are doubles
  if(var1->type == DOUBLE && var2->type == DOUBLE) {
    result_of_addition->type=DOUBLE;
    result_of_addition->data.floatingpoint=var1->data.floatingpoint - var2->data.floatingpoint;

  } else if(var1->type == DOUBLE) {
    int valueOfvar2=0;
    //if its a string then we know its a char (strlen == 1)
    if(var2->type == STRING) 
      valueOfvar2= (int) var1->data.str->string[0];
    else if(var2->type == INTEGER) 
      valueOfvar2=var1->data.integer;
    else if(var2->type == BOOL) 
      valueOfvar2=var1->data.boolean;
    
    //performs computation
    result_of_addition->type=DOUBLE;
    result_of_addition->data.floatingpoint=var1->data.floatingpoint - (double) valueOfvar2;

  } else if(var2->type == DOUBLE) {
    int valueOfvar1=0;
    //if its a string then we know its a char (strlen == 1)
    if(var2->type == STRING) 
      valueOfvar1= (int) var1->data.str->string[0];
    else if(var2->type == INTEGER) 
      valueOfvar1=var1->data.integer;
    else if(var2->type == BOOL) 
      valueOfvar1=var1->data.boolean;
    
    //performs computation
    result_of_addition->type=DOUBLE;
    result_of_addition->data.floatingpoint=valueOfvar1 - result_of_addition->data.floatingpoint;
  } else {
    int valueOfvar1=0;
    int valueOfvar2=0;

    //these if statements sets the integer values of the integer fields
    if(var2->type == STRING) 
      valueOfvar2=(int) var1->data.str->string[0];
    else if(var2->type == INTEGER) 
      valueOfvar2=var1->data.integer;
    else if(var2->type == BOOL) 
      valueOfvar2=var1->data.boolean;

    if(var1->type == STRING) 
      valueOfvar1=(int) var1->data.str->string[0];
    else if(var1->type == INTEGER) 
      valueOfvar1=var1->data.integer;
    else if(var1->type == BOOL) 
      valueOfvar1=var1->data.boolean;

    result_of_addition->type=INTEGER;
    //computes the addition
    result_of_addition->data.integer= valueOfvar2 - valueOfvar1;
  }

  return result_of_addition;
}

//if multiplication of these two variables structs is invalid
//then this function will throw an error
//NOTICE: the returned Variable struct as a NULL pointer for name
//make sure to to take this into account
Variable* MultiplyVariables(Variable* var1, Variable *var2) {
  if(areVariableTypesValidForArithmeticComputation(var1,var2) == 0) 
    return NULL;

  Variable* result_of_mult=malloc(sizeof(Variable));
  result_of_mult->name=NULL;
  if(var1->type == DOUBLE && var2->type == DOUBLE) {
    result_of_mult->type=DOUBLE;
    result_of_mult->data.floatingpoint=var1->data.floatingpoint * var2->data.floatingpoint;
  } else if(var1->type == DOUBLE) {
    int valueOfvar2=0;
    
    //if its a string then we know its a char (strlen == 1)
    if(var2->type == STRING) 
      valueOfvar2= (int) var1->data.str->string[0];
    else if(var2->type == INTEGER) 
      valueOfvar2=var1->data.integer;
    else if(var2->type == BOOL) 
      valueOfvar2=var1->data.boolean;
    
    result_of_mult->type=DOUBLE;
    //computes the multiplication
    result_of_mult->data.floatingpoint= var1->data.floatingpoint * (double)valueOfvar2;

    return result_of_mult;

  } else if(var2->type == DOUBLE) {
    int valueOfvar1=0;

    //if its a string then we know its a char (strlen == 1)
    if(var1->type == STRING) 
      valueOfvar1=(int) var1->data.str->string[0];
    else if(var1->type == INTEGER) 
      valueOfvar1=var1->data.integer;
    else if(var1->type == BOOL) 
      valueOfvar1=var1->data.boolean;
    
    result_of_mult->type=DOUBLE;
    //computes the multiplication
    result_of_mult->data.floatingpoint= var2->data.floatingpoint * (double)valueOfvar1;

    return result_of_mult;

  //else the resulting multiplication will be a integer
  } else {
    int valueOfvar1=0;
    int valueOfvar2=0;

    //these if statements sets the integer values of the integer fields
    if(var2->type == STRING) 
      valueOfvar2=(int) var1->data.str->string[0];
    else if(var2->type == INTEGER) 
      valueOfvar2=var1->data.integer;
    else if(var2->type == BOOL) 
      valueOfvar2=var1->data.boolean;

    if(var1->type == STRING) 
      valueOfvar1=(int) var1->data.str->string[0];
    else if(var1->type == INTEGER) 
      valueOfvar1=var1->data.integer;
    else if(var1->type == BOOL) 
      valueOfvar1=var1->data.boolean;

    result_of_mult->type=INTEGER;
    //computes the multiplication
    result_of_mult->data.integer= valueOfvar2 * valueOfvar1;
  }
  return result_of_mult;
}



