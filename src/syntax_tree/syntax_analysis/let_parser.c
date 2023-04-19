#include "../../utils/lineparsing.h"
#include "../../types/types.h"
#include "../syntax_tree.h"
#include "../../types/type_parser.h"
#include "./let_parser.h"
#include "../../types/lists.h"
#include "../../symbol_tables/variable_table.h"

// typedef struct{
//   char* var_name;
//   TYPE data_type;
//   Variable* var;
// } let_node_instruction;


typedef struct {
  char* name;
  int length;
} var_name;

static var_name* getValidVarName(char* line);
static int checkSyntaxEqualSymbol(char* line);
static TYPE AssignType(let_node_instruction *let_,char* line);
static TYPE AssignInteger(let_node_instruction *let_,char* line);
static TYPE AssignDouble(let_node_instruction *let_,char* line);
static TYPE AssignString(let_node_instruction *let_,char* line);
static TYPE AssignArray(let_node_instruction *let_,char* line);
static TYPE AssignBool(let_node_instruction *let_,char* line);
static TYPE AssignNull(let_node_instruction *let_,char* line);
static TYPE AssignFunc(let_node_instruction *let_, char* line);
static TYPE AssignVar(let_node_instruction *let_, char* line);
static TYPE AssignArithmeticExpression(let_node_instruction *let_, char* line);
static TYPE AssignBoolExpression(let_node_instruction *let_, char* line);


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
    free(let_node);
    return INVALID_TYPE;
  }
  
  free(var_name);
  
  struct Instruction *instruction_node = malloc(sizeof(struct Instruction));
  instruction_node->next=NULL;
  instruction_node->command = LET;
  instruction_node->command_data=let_node;
  
  addCommmand(list,instruction_node);
  addVariable_to_VarTable(let_node->var);
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
/*
will check if '=' is the next none whitespace char
will return 1 if true
0 if not
*/
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

//creates the proper variable struct for the let_node_instructions struct
static TYPE AssignType(let_node_instruction *let_,char* line) {
  let_->data_type=getTypeHint(line);
  switch(let_->data_type) {
    case INTEGER:
      return AssignInteger(let_,line);
    case DOUBLE:
      return AssignDouble(let_,line);
    case STRING:
      return AssignString(let_, line);
    //TODO
    case ARRAY:
      return AssignArray(let_,line);
    case _NULL:
      return AssignNull(let_,line);
    case BOOL:
      return AssignBool(let_,line);
    case FUNCTION:
      return AssignFunc(let_,line);
    case VAR:
      return AssignVar(let_,line);   
    //TODO
    case ARITHMETIC_EXPRESSION:
      return AssignArithmeticExpression(let_,line);
    //TODO
    case BOOL_EXPRESSION:
      return AssignBoolExpression(let_,line);
    case UNKNOWN:
      break;
    default:
      break;
  } 
  return UNKNOWN;
}

/*
parses integer value and mallocs variable struct 
and assigns it to let instructions struct var field
return UNKNOWN IF syntax is not proper
*/
static TYPE AssignInteger(let_node_instruction *let_,char* line) {
  int *integer=getInteger(line); //gets the integer to be assigned
  if(integer == NULL) { 
    free(integer);
    return UNKNOWN;
  }
  let_->var=createVariableStruct(INTEGER,let_->var_name,integer,-1);
  free(integer);
  printf("%s: %d \n",let_->var_name, let_->var->data.integer);
  return INTEGER;
}

/*
parses floating point value and mallocs variable struct 
and assigns it to let instructions struct var field
return UNKNOWN if syntax is not proper 
*/
static TYPE AssignDouble(let_node_instruction *let_,char* line) {
  double *double_precision=getDouble(line);
  if(double_precision == NULL) {
    free(double_precision);
    return UNKNOWN;
  }
  let_->var=createVariableStruct(DOUBLE,let_->var_name,double_precision,-1);
  free(double_precision);
  printf("%s: %f \n", let_->var_name ,let_->var->data.floatingpoint);
  return DOUBLE;

}

/*
parses string within "" and mallocs variable struct 
and assigns it to let instructions struct var field
return UNKNOWN IF syntax is not proper
*/
static TYPE AssignString(let_node_instruction *let_,char* line) {
  char* str=getStringFromDelimiter_withTrailingWhitespace(line,'"');
  if(str == NULL) {
    free(str); 
    return UNKNOWN;
  }
  let_->var=createVariableStruct(STRING,let_->var_name,str,strlen(str));
  printf("%s: %s \n", let_->var_name, let_->var->data.str->string);
  return STRING;
}

/*
parses array within [] and mallocs variable struct and assigns it to let instructions struct var field
return UNKNOWN IF syntax is not proper
TODO
*/
static TYPE AssignArray(let_node_instruction *let_,char* line) {
  while(*line != '[' && *line != '\0') 
    line++;
  printf("1");
  if(*line == '\0') return UNKNOWN;

  while(*line != ']' && *line != '\0')
    line++;
  
  if(*line == '\0') return UNKNOWN;
  
  printf("array");

  //initial size is 8 by default
  List* new_array=createList(8);

  let_->var=createVariableStruct(ARRAY,let_->var_name,new_array,-1);
  return ARRAY;
}

/*
parses bool value and mallocs variable struct 
and assigns it to let instructions struct var field
return UNKNOWN IF syntax is not proper
*/
static TYPE AssignBool(let_node_instruction *let_,char* line) {
  int num=-1;
  if(checkAssignmentSyntax(line, "false") == 1) {
    num=0;
    let_->var=createVariableStruct(BOOL,let_->var_name,&(num),-1);
  } else if(checkAssignmentSyntax(line, "true") == 1) {
    num=1;
    let_->var=createVariableStruct(BOOL,let_->var_name,&(num),-1);
  } else {
    return UNKNOWN;
  }
  printf("%s: %d\n",let_->var_name, let_->var->data.boolean);
  return BOOL;
}

/*
parses NULL value and mallocs variable struct 
and assigns it to let instructions struct var field
return UNKNOWN IF syntax is not proper
*/
static TYPE AssignNull(let_node_instruction *let_,char* line) {
  if(checkAssignmentSyntax(line, "null") == 0) return UNKNOWN;
  let_->var=createVariableStruct(_NULL,let_->var_name,NULL,-1);
  printf("%s: %s is NULL\n", let_->var_name, let_->var_name);
  return _NULL;
}

/*
parses NULL value and mallocs variable struct 
and assigns it to let instructions struct var field
return UNKNOWN if syntax is not proper
TODO
*/
static TYPE AssignFunc(let_node_instruction *let_, char* line) {      
  printf("this: %s is a function", line);
  return FUNCTION;
}


/*
Parses variable name and assigns it to the let instructions struct var field
returns UNKNOWN if syntax is not proper
*/
static TYPE AssignVar(let_node_instruction *let_, char* line) {      
  String* var_name = getVariableName(line,1);
  //if getVariableAssignmentName(line,1) return NULL, means syntax is incorrect
  if(var_name == NULL) return UNKNOWN;
  Variable* var = createVariableStruct(VAR, let_->var_name,var_name->string,var_name->length);
  free(var_name); //frees String struct (not the actual malloced var_name->string)
  let_->var=var;

  printf("%s: var %s", let_->var_name,let_->var->data.str->string);
  return VAR;
}

//TODO
static TYPE AssignArithmeticExpression(let_node_instruction *let_, char* line) {
  char math_word[]="math";
  int index_count=0;
  //gets the first 4 non whitespace chars to check if proper "math" expression is encountered
  for(int i=0; line[i] != '\0' && index_count < 4; i++) {
    if(isspace(line[i]) != 0) {
      continue;
    } else if(line[i] == math_word[index_count]) {
      index_count++;
    } else {
      return UNKNOWN;
    }
  }

  //TODO --> WRITE CODE FOR PARSING ARITHMETIC EXPRESSION
  char* arithmetic_expression = getStringFromDelimiter(line,'(',')',1);

  printf("\nmath Expression: %s", arithmetic_expression); //for testing
  free(arithmetic_expression);
  return ARITHMETIC_EXPRESSION;
}

//TODO
static TYPE AssignBoolExpression(let_node_instruction *let_, char* line) {
  char bool_word[]="bool";
  int index_count=0;
  //gets the first 4 non whitespace chars to check if proper "bool" expression is encountered
  for(int i=0; line[i] != '\0' && index_count < 4; i++) {
    if(isspace(line[i]) != 0) {
      continue;
    } else if(line[i] == bool_word[index_count]) {
      index_count++;
    } else {
      return UNKNOWN;
    }
  }
  //TODO --> WRITE CODE FOR PARSING BOOLEAN EXPRESSION

  char* arithmetic_expression = getStringFromDelimiter(line,'(',')',1);

  printf("\nBool Expression: %s", arithmetic_expression); //for testing
  free(arithmetic_expression);
  return BOOL_EXPRESSION;
}

