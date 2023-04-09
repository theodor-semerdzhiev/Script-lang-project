#include "types.h"
#include "../syntax_tree/lineparsing.h"
#include <string.h>

typedef enum{FALSE=0, TRUE=1} boolean;


//this function given the input is a pointer to a string containing some variable assignment (after the '=')
//return UNKNOWN if there is a syntax error
//this function stops after seeing a '[', a number or '"'
//it does NOT check if syntax is proper

//TODO: this function is not finished
TYPE getVariableType(char* line) {
  boolean metNumber=FALSE;
  for(int i=0; line[i] != '\0' ; i++) {
    if(isalpha(line[i]) != 0) 
      return UNKNOWN;
    if(line[i] == '[') 
      return ARRAY;
    if(line[i] == '"') 
      return STRING;
    if(line[i] == '.' && metNumber == TRUE) 
      return FLOAT;
    if(isdigit(line[i]) != 0) {
      metNumber=TRUE;
      continue;
    }
    return UNKNOWN;
  }
}


//used to extract String enclosed in a quotation assignment
//char* line must point after the '=' symbol
static char* getStringFromQuotationMarks(char *line) {
  int str_len=0;
  boolean asmetQuotationMark=FALSE;

  for(int i=0; line[i] != '"' || asmetQuotationMark==FALSE; i++) {
    str_len++;
    //if " " is never closed
    if(line[i] == '\0') 
      return NULL;
    if(line[i] == '"') 
      asmetQuotationMark=TRUE;
  }
  //checks if a invalid token is present
  if(isLineEmpty(line+str_len+1) == 0) return NULL;
  char* parsed_str=malloc(sizeof(char)*str_len+1);
  for(int i=0; i < str_len; i++) {
    parsed_str[i]=line[i];
  }
  parsed_str[str_len]='\0';
  return parsed_str;
}

//TODO
static int getInteger(char *line) {

}

//TODO
static float getFloat(char *line) {

}


