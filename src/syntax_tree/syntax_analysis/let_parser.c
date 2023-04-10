#include "../lineparsing.h"
#include "../../types/types.h"
#include "../syntax_tree.h"
#include "../../types/type_parser.h"

typedef struct{
  char* var_name;
  TYPE data_type;
  void* data;
} let_node_instruction;

PARSER_EXIT_CODE create_let_instruction(CommandList *list, char* line);
static void** getValidVarName(char* line);
static int checkSyntaxEqualSymbol(char* line);
static TYPE AssignType(let_node_instruction *let_,char* line);

PARSER_EXIT_CODE create_let_instruction(CommandList *list, char* line) {
  char* line_ptr=&(line[4]);
  void** var_info = getValidVarName(line_ptr);
  // printf("%s %d\n", *(char**) var_info[0], *(int*) var_info[1]);
  
  if(var_info == NULL) // if variable name is invalid
    return INVALID_VAR; 
  
  line_ptr+=*(int*)var_info[1];
  int char_lengths_to_equal_symbol=checkSyntaxEqualSymbol(line_ptr);

  // if = is not present after variable name
  if(char_lengths_to_equal_symbol == -1) 
    return INVALID_SYNTAX;


  char * test= *(char**) var_info[0];
  let_node_instruction *let_node = malloc(sizeof(let_node_instruction));
  let_node->var_name= test;
  let_node->data_type=INTEGER; //TEMPORARY WILL BE SET TO SOMETHING APPROPRIATE
  let_node->data=NULL;//TEMPORARY WILL BE SET TO SOMETHING

  line_ptr+=char_lengths_to_equal_symbol+1;
  
  if(AssignType(let_node, line_ptr) == UNKNOWN) 
    return INVALID_TYPE;
  
  struct Instruction *instruction_node = malloc(sizeof(struct Instruction));
  instruction_node->next=NULL;
  instruction_node->command = LET;
  instruction_node->command_data=let_node;
  
  addCommmand(list,instruction_node);

  free(var_info[1]);
  free(var_info);

  return CLEAN_EXIT;
}


//will return NULL if not
//return an array [char* name, int* length]
static void** getValidVarName(char* line) {
  //first character of variable name must be 
  if(isalpha(line[0]) == 0) 
    return NULL;
  char* str_ptr=line;
  for(int i=0; line[i] != '\0'; i++) {
    if(isalpha(line[0]) == 0 && isspace(line[0]) != 0) { 
      return NULL;
    } else if(isalpha(line[0]) != 0) {
      str_ptr+=i;
      break;
    }
  }

  int var_len=0;
  for(int i=0; isalnum(str_ptr[i]) != 0; i++) var_len++;

  char *varname=mallocString(str_ptr,var_len);
  int *return_int=malloc(sizeof(int));
  *return_int=var_len;
  
  void** return_arr = malloc(sizeof(void*)*2);

  return_arr[0]=&varname; 
  return_arr[1]=return_int;
  return return_arr;
}

//will check if '=' is the next none whitespace char
//will return 1 if true
//0 if not
static int checkSyntaxEqualSymbol(char* line) {
  for(int i=0;  line[i] != '\0'; i++) {
    if(line[i]== '=') {
      return i;
    } else if(isspace(line[i]) == 0) {
      return -1;
    }
  }
  return -1;
}

static TYPE AssignType(let_node_instruction *let_,char* line) {
   switch(getVariableType(line)) {
    case INTEGER:
      let_->data_type=INTEGER;
      let_->data=(int*) getInteger(line);
      if(let_->data == NULL) 
        break;
      printf("integer: %d \n", *(int*)let_->data);
      return INTEGER;
      
    case DOUBLE:
      let_->data_type=DOUBLE;
      let_->data=(double*) getDouble(line);
      if(let_->data == NULL) 
        break;
      printf("double: %f \n", *(double*)let_->data);
      return DOUBLE;

    case STRING:
      let_->data_type=STRING;
      let_->data= (char*) getStringFromQuotationMarks(line);
      if(let_->data == NULL)
        break;
      printf("string: %s \n", (char*)let_->data);
      return STRING;
    //TODO

    case ARRAY:
      let_->data_type=ARRAY;
      printf("this: %s is and array", line);
      return ARRAY;
    
    case FUNCTION:
      let_->data_type=FUNCTION;
      printf("this: %s is a function", line);
      return FUNCTION;
    
    case VAR:
      let_->data_type=VAR;
      printf("this: %s is a var", line);
      return VAR;
      
    case UNKNOWN:
      break;
    default:

  } 
  free(let_->var_name);
  free(let_);
  return UNKNOWN;
}













