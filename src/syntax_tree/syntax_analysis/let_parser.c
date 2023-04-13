#include "../lineparsing.h"
#include "../../variables/types.h"
#include "../syntax_tree.h"
#include "../../variables/variable_parser.h"

typedef struct{
  char* var_name;
  TYPE data_type;
  Variable* var;
} let_node_instruction;


typedef struct {
  char* name;
  int length;
} var_name;

PARSER_EXIT_CODE create_let_instruction(CommandList *list, char* line, int lineNumber);
static var_name* getValidVarName(char* line);
static int checkSyntaxEqualSymbol(char* line);
static TYPE AssignType(let_node_instruction *let_,char* line);

PARSER_EXIT_CODE create_let_instruction(CommandList *list, char* line, int lineNumber) {
  char* line_ptr=&(line[4]);
  var_name* var_name = getValidVarName(line_ptr);
  
  if(var_name == NULL) { // if variable name is invalid
    return INVALID_VAR; 
  }
  
  line_ptr+=var_name->length;
  int char_lengths_to_equal_symbol=checkSyntaxEqualSymbol(line_ptr);

  // if = is not present after variable name
  if(char_lengths_to_equal_symbol == -1) {
    free(var_name->name);
    free(var_name);
    return INVALID_SYNTAX;
  }

  let_node_instruction *let_node = malloc(sizeof(let_node_instruction));
  let_node->var_name= var_name->name;

  line_ptr+=char_lengths_to_equal_symbol+1;
  
  if(AssignType(let_node, line_ptr) == UNKNOWN) {
    free(var_name->name);
    free(var_name);
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
  variable_struct->name=varname;

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
  let_->data_type=getAssignmentType(line);
  switch(let_->data_type) {
    case INTEGER:
      int *integer=getInteger(line); //gets the integer to be assigned
      if(integer == NULL) { 
        free(integer);
        break;
      }
      let_->var=createVariableStruct(INTEGER,let_->var_name,integer,-1);
      free(integer);
      printf("integer: %d \n", let_->var->data.integer);
      return INTEGER;
    case DOUBLE:
      double *double_precision=getDouble(line);
      if(double_precision == NULL) {
        free(double_precision);
        break;
      }
      let_->var=createVariableStruct(DOUBLE,let_->var_name,double_precision,-1);
      free(double_precision);
      printf("double: %f \n", let_->var->data.floatingpoint);
      return DOUBLE;
    case STRING:
      char* str=getStringFromQuotationMarks(line);
      if(str == NULL) {
        free(str); 
        break;
      }
      let_->var=createVariableStruct(STRING,let_->var_name,str,strlen(str));
      printf("string: %s \n", let_->var->data.str->string);
      return STRING;
    //TODO
    case ARRAY:
      printf("Array: %s ", line);
      return ARRAY;
    
    case _NULL:
      if(checkAssignmentSyntax(line, "null") == 0) 
        break;
      let_->var=NULL;
      
      printf("NULL: %s is NULL\n", let_->var_name);
      return _NULL;
    case BOOL:
      int num=-1;
      if(checkAssignmentSyntax(line, "false") == 1) {
        num=0;
        let_->var=createVariableStruct(BOOL,let_->var_name,&(num),-1);
      } else if(checkAssignmentSyntax(line, "true") == 1) {
        num=1;
        let_->var=createVariableStruct(BOOL,let_->var_name,&(num),-1);
      } else {
        break;
      }
      printf("BOOLEAN: %d\n", let_->var->data.boolean);
      return BOOL;

    case FUNCTION:
      printf("this: %s is a function", line);
      return FUNCTION;
    
    case VAR:
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
