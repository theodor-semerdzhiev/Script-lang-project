#include "types.h"
#include "../syntax_tree/lineparsing.h"
#include <string.h>

typedef enum{FALSE=0, TRUE=1} boolean;

TYPE getVariableType(const char* line);
char* getStringFromQuotationMarks(char *line);
int* getInteger(const char* line);
double* getDouble(const char *line);

//this function given the input is a pointer to a string containing some variable assignment (after the '=')
//return UNKNOWN if there is a syntax error
//this function stops after seeing a '[', a number or '"'
//it does NOT check if syntax is proper
TYPE getVariableType(const char* line) {
  boolean metNumber=FALSE;
  for(int i=0; line[i] != '\0' ; i++) {
    if(isspace(line[i]) != 0)
      continue;
    if(isalpha(line[i]) != 0) 
      return UNKNOWN;
    if(line[i] == '[') 
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


//used to extract String enclosed in a quotation assignment
//char* line must point after the '=' symbol
//returned string is malloced
char* getStringFromQuotationMarks(char *line) {
  int str_len=0;
  char *line_ptr=line;
  boolean asmetQuotationMark=FALSE;

  for(int i=0; line[i] != '"' || asmetQuotationMark==FALSE; i++) {
    if(asmetQuotationMark == TRUE) str_len++;
    //if " " is never closed
    if(line[i] == '\0') 
      return NULL;
    if(line[i] == '"') {
      asmetQuotationMark=TRUE;
      line_ptr++;
    } else if(asmetQuotationMark == FALSE) {
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




