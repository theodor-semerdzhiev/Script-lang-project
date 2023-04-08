#include "../lineparsing.h"
#include "../../types/types.h"
#include "../syntax_tree.h"

typedef struct{
  char* var_name;
  TYPE data_type;
  void* data;
} let_node_instruction;

static void** getValidVarName(char* line);
static int checkSyntaxEqualSymbol(char* line);

PARSER_EXIT_CODE create_let_instruction(CommandList *list, char* line) {

  void** var_info = getValidVarName(&(line[4]));
  if(var_info == NULL) return INVALID_VAR; // if variable name is invalid
  
  int char_lengths_to_equal_symbol=checkSyntaxEqualSymbol(&(line[4 + *(int*)var_info[1]]));

  // if = is not present after variable name
  if(char_lengths_to_equal_symbol == -1) return INVALID_SYNTAX; 

  char * test= *(char**) var_info[0];
  let_node_instruction *let_node = malloc(sizeof(let_node_instruction));
  let_node->var_name= test;
  let_node->data_type=INTEGER; //TEMPORARY WILL BE SET TO SOMETHING APPROPRIATE
  let_node->data=NULL;//TEMPORARY WILL BE SET TO SOMETHING

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









