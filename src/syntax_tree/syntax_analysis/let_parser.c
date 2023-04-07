#include "../lineparsing.h"
#include "../../types/types.h"
#include "../syntax_tree.h"

typedef struct{
  char* var_name;
  TYPE data_type;
  void* data;
} let_intruc;

PARSER_EXIT_CODE create_let_instruction(CommandList list, char* line) {
  
  int* line_info = getLineCounts(line);
  char **parsed_line = parseLine(line,line_info[0], line_info[1]);
  
  void** var_info = getValidVarName(parsed_line[2]);

  if(var_info == NULL) return INVALID_VAR; // if variable name is invalid
  
  int char_lengths_to_equal_symbol=checkSyntaxEqualSymbol(line+*(int*)var_info[1]);

  if(char_lengths_to_equal_symbol == -1) return INVALID_SYNTAX; // if = is not present after variable name
  
  //then must check for type and create appropriate struct
  //also create variable hashmap to store var names and there data

  free(line_info);
  freeArrayOfStrings(parsed_line);

  return CLEAN_EXIT;
}


//will return NULL if not
//return an array [char* name, int* length]
static void** getValidVarName(char* line) {
  if(isalpha(line[0])==0) return NULL;

  int var_len=0;
  for(int i=0; isalnum(line[i]) != 0; i++) var_len++;

  char *varname=mallocString(line,var_len);
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









