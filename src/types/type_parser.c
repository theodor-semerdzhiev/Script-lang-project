#include "types.h"
#include "../syntax_tree/lineparsing.h"
#include <string.h>

typedef enum{FALSE=0, TRUE=1} boolean;

TYPE getAssignmentType(const char* line);
char* getStringFromDelimiter_withTrailingWhitespace(char *line, char delimiter);
int* getInteger(const char* line);
double* getDouble(const char *line);
int checkAssignmentSyntax(const char* line, const char* assignment);
String* getVariableAssignmentName(char* line, int checkTrailingWhitespace);

//this function given the input is a pointer to a string containing some variable assignment (after the '=')
//return UNKNOWN if there is a syntax error
//this function stops after seeing a '[', a number or '"'
//it does NOT check if syntax is proper
TYPE getAssignmentType(const char* line) {
  boolean metNumber=FALSE;
  for(int i=0; line[i] != '\0' ; i++) {
    if(isspace(line[i]) != 0)
      continue;
    if(line[i] == 'n') 
      return _NULL;
    if(line[i] == 't' || line[i] =='f')
      return BOOL;
    if(line[i] == 'm')
      return ARITHMETIC_EXPRESSION;
    if(line[i] == 'b')
      return BOOL_EXPRESSION;
    if(isalpha(line[i]) != 0) 
      return UNKNOWN;
    if(line[i] == '[') //might change
      return ARRAY;
    if(line[i] == '$')
      return VAR;
    if(line[i] == '"') 
      return STRING;
    if(line[i] == '.' && metNumber == TRUE) 
      return DOUBLE;
    if(isdigit(line[i]) != 0) {
      metNumber=TRUE;
      continue;
    }
    return UNKNOWN;
  }
  return INTEGER;
}


// used to extract String enclosed in a delimiter
// char* line must point after the '=' symbol
// returned string is malloced
char* getStringFromDelimiter_withTrailingWhitespace(char *line, char delimiter) {
  int str_len=0;
  char *line_ptr=line;
  boolean asmetDelimiter=FALSE;

  for(int i=0; line[i] != delimiter || asmetDelimiter==FALSE; i++) {
    if(asmetDelimiter == TRUE) str_len++;
    //if " " is never closed
    if(line[i] == '\0') 
      return NULL;
    if(line[i] == delimiter) {
      asmetDelimiter=TRUE;
      line_ptr++;
    } else if(asmetDelimiter == FALSE) {
      line_ptr++;
    }
  }
  //checks if a invalid token is present
  if(isLineEmpty(line_ptr+str_len+1) == 0) return NULL;
  char* parsed_str=malloc(sizeof(char)*str_len+1);
  for(int i=0; i < str_len; i++) {
    parsed_str[i]=line_ptr[i];
  }
  parsed_str[str_len]='\0';
  return parsed_str;
}

//parses integer
//designed for the LET and SET keywords
int* getInteger(const char* line) {
  boolean asMetNumber=FALSE;
  for(int i=0; line[i] != '\0'; i++) {
    //if its whitespace and we have not yet met a number
    if(isspace(line[i]) != 0 && asMetNumber == FALSE) {
      continue;
    } else if(isspace(line[i]) != 0 && asMetNumber == TRUE) {
      //if no more characters are present after first number token
      if(isLineEmpty(&(line[i])) == 1)
        break;
      else 
        return NULL;
    //if we encounter non valid character
    } else if(isdigit(line[i]) == 0) return NULL;
    //otherwise we have encountered a valid digit character
    asMetNumber=TRUE;
  }
  int* variable_value = malloc(sizeof(int)); 
  *variable_value=atoi(line); 
  return variable_value;
}

//gets double from assignment operators
double* getDouble(const char *line) {
  boolean asMetNumber=FALSE;
  boolean asMetDot=FALSE;
  for(int i=0; line[i] != '\0'; i++) {
    //if we encounter non valid characters
    if(line[i] == '.' && asMetDot == FALSE) {
      asMetDot=TRUE;
      continue;
    } 
    //we cannot have multiple dots
    if(line[i] =='.' && asMetDot == TRUE) {
      return NULL;
    } 
    if(isspace(line[i]) != 0 && asMetNumber == FALSE) {
      continue;
    } else if(isspace(line[i]) != 0 && asMetNumber == TRUE) {
      if(isLineEmpty(&(line[i])) == 1) 
        break;
      else
        return NULL;
    } 
    if(isdigit(line[i]) == 0) return NULL;
    asMetNumber=TRUE;
  }
  
  double * variable_value = malloc(sizeof(double));
  *variable_value=atof(line);
  return variable_value;
}

//returns 1 if assignment word (null, or true or false) is the only word after '='
//returns 0 otherwise
int checkAssignmentSyntax(const char* line, const char* assignment) {
  int offset=0;
  for(int i=0; isspace(line[i]) != 0; i++) {
    offset++;
  }
  char *null_str=getNthToken(line+offset,1);
  if(strcmp(null_str, assignment) != 0 || isLineEmpty(line+offset+strlen(assignment)) == 0) {
    free(null_str);
    return 0;
  }
  free(null_str);
  return 1;
}

//gets a variable name (ex: $num)
//stores it in a String struct
//checkTrailingWhitespace checks for an empty line after the variable name (0 = no check, 1 = check)
//make sure to call free on the string struct properly 
String* getVariableAssignmentName(char* line, int checkTrailingWhitespace) {
  boolean asMetDollar=FALSE; //keeps track if we have met a $ sign
  char* str_ptr=line; 
  int variable_name_length=0;

  for(int i=0; line[i] != '\0'; i++) {
    if(line[i] == '$') {
      asMetDollar=TRUE;
      str_ptr++;
      continue;
    } else if(asMetDollar == TRUE && isspace(line[i]) != 0) {
      break;
    } else if(asMetDollar == TRUE && isalnum(line[i]) == 0) {
      break;
    } else if(asMetDollar == TRUE) {
      variable_name_length++;
    //if asMetDollar is FALSE but we have encountered character
    } else if(isspace(line[i]) == 0) {
      return NULL;
    } else {
      str_ptr++;
    }
  }
  if(isalpha(str_ptr[0]) == 0) return NULL;
  if(checkTrailingWhitespace == 1 && isLineEmpty(str_ptr+variable_name_length) == 0) return NULL;

  char* var_name=malloc(sizeof(char)*variable_name_length+1);
  for(int i=0; i<variable_name_length; i++) {
    var_name[i]=str_ptr[i];
  }
  var_name[variable_name_length]='\0';

  String* str=malloc(sizeof(String));
  str->string=var_name;
  str->length=variable_name_length;
  return str;
}


