#include "../lineparsing.h"
#include "../../variables/types.h"
#include "../syntax_tree.h"
#include "../../variables/variable_parser.h"

typedef struct{
  char* var_name;
  TYPE data_type;
  void* data;
} let_node_instruction;


typedef struct {
  char* var_name;
  int length;
} var_name;

PARSER_EXIT_CODE create_let_instruction(CommandList *list, char* line, int lineNumber);
static var_name* getValidVarName(char* line);
static int checkSyntaxEqualSymbol(char* line);
static TYPE AssignType(let_node_instruction *let_,char* line);

PARSER_EXIT_CODE create_let_instruction(CommandList *list, char* line, int lineNumber) {
  char* line_ptr=&(line[4]);
  var_name* variable = getValidVarName(line_ptr);
  
  if(variable == NULL) // if variable name is invalid
    return INVALID_VAR; 
  
  line_ptr+=variable->length;
  int char_lengths_to_equal_symbol=checkSyntaxEqualSymbol(line_ptr);

  // if = is not present after variable name
  if(char_lengths_to_equal_symbol == -1) {
    free(variable->var_name);
    free(variable);
    return INVALID_SYNTAX;
  }

  let_node_instruction *let_node = malloc(sizeof(let_node_instruction));
  let_node->var_name= variable->var_name;
  let_node->data_type=INTEGER; //TEMPORARY WILL BE SET TO SOMETHING APPROPRIATE
  let_node->data=NULL;//TEMPORARY WILL BE SET TO SOMETHING

  line_ptr+=char_lengths_to_equal_symbol+1;
  
  if(AssignType(let_node, line_ptr) == UNKNOWN) {
    free(variable->var_name);
    free(variable);
    return INVALID_TYPE;
  }
  
  struct Instruction *instruction_node = malloc(sizeof(struct Instruction));
  instruction_node->next=NULL;
  instruction_node->command = LET;
  instruction_node->command_data=let_node;
  
  addCommmand(list,instruction_node);

  return CLEAN_EXIT;
}


//will return NULL if not
//return an array [char* name, int* length]
static var_name* getValidVarName(char* line) {
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
  for(int i=0; isalnum(str_ptr[i]) != 0 || line[i] =='_'; i++) 
    var_len++;

  char *varname=mallocString(str_ptr,var_len);
  
  var_name* variable_struct=malloc(sizeof(var_name));
  variable_struct->length=var_len;
  variable_struct->var_name=varname;

  return variable_struct;
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
   switch(getAssignmentType(line)) {
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
      printf("Array: %s ", line);
      return ARRAY;
    
    case _NULL:
      let_->data_type=_NULL;
      let_->data=NULL;
      if(checkAssignmentSyntax(line, "null") == 0)
        break;
      printf("NULL: %s is NULL", let_->var_name);
      return _NULL;
    case BOOL:

      return BOOL;
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

