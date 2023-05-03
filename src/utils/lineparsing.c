#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "../types/types.h"
#include "../bool.h"
#include "lineparsing.h"

//Returns an array of strings corresponding to each token in the input string 
//Make sure to call freeArrayOfString(char**, int) to free returned char** array
char** tokenizeString(char* str, int num_of_tokens, int length_str) {
  if(num_of_tokens == 0 || length_str == 0) return NULL;
  char *tmp_ptr=str;
  char ** parsed_line=malloc(sizeof(char*)*num_of_tokens+1);
  int count=0;
  while(count < num_of_tokens) {
    tmp_ptr+=getCharsBeforeWhiteSpace(tmp_ptr);
    int token_length=getTokenLength(tmp_ptr);
    parsed_line[count]=mallocString(tmp_ptr,token_length);
    tmp_ptr+=token_length;
    count++;
  }
  parsed_line[count]=NULL;
  return parsed_line;
}

//frees an array of Strings
//useful to be called after char** parseLine
void freeArrayOfStrings(char** str_arr) {
  for(int i=0; str_arr[i] != NULL; i++) {
    free(str_arr[i]);
  }
}

//this functions returns the number of characters before a whitespace character
int getCharsBeforeWhiteSpace(const char * tmp_ptr){
  if(isspace(tmp_ptr[0]) != 0) {
    int i=0;
    for(i=0; isspace(tmp_ptr[i]) != 0; i++) {}
    return i;
  }
  return 0;
}

//gets the amount of chars until a whitespace character or a null character
int getTokenLength(const char* start) {
  int token_len=0;
  for(int i=0; start[i] != '\0' && isspace(start[i]) == 0; i++) {
    token_len++;
  } 
  return token_len;
}

//takes an initial index str and creates a substring of length int length
char* mallocString(const char* str, unsigned int length) {
  char *return_str=malloc(sizeof(char)*length+1);
  for(int i=0; i < length; i++) return_str[i]=str[i];
  return_str[length]='\0';
  return return_str;
}

// return the number of tokens and the length of the string in an array respectively
// [# of tokens, # length of str] 
// length of str does not include the null char
int* getLineCounts(const char* str) {
  boolean met_token=FALSE;
  int * return_arr = malloc(sizeof(int)*2);
  return_arr[0]=0;
  return_arr[1]=0;
  for(int i=0; str[i] != '\0'; i++) {
    // if *ptr is a white space character
    if(isspace(str[i]) != 0 && met_token != FALSE) {
      return_arr[0]++;
      met_token=FALSE;
    } else if(isspace(str[i]) == 0) {
      met_token=TRUE;
    }
    return_arr[1]++;
  }
  if(met_token == TRUE) return_arr[0]++;
  return return_arr;
}

//gets the nth token from a string
//0 is invalid input
//make sure to free string after use
char* getNthToken(const char* str, int position) {  
  boolean met_token=FALSE;
  int tokencount=0;
  int startindex=-1;
  int str_len=0;
  for(int i=0; str[i] != '\0'; i++) {
    // if *ptr is white space character
    if(isspace(str[i]) != 0 && met_token == TRUE) {
      tokencount++;
      if(tokencount==position){
        return mallocString(&(str[startindex]),str_len);
      }
      met_token=FALSE;
    } else if(isspace(str[i]) == 0) {
      if(tokencount==position-1) {
        if(met_token != TRUE) startindex=i;
        str_len++;
      }
      met_token=TRUE;
    }
  }
  if(startindex != -1) {
    return mallocString(&(str[startindex]), str_len);
  }
  return NULL;
}

//will return 1 if line is white space, 0 if not
int isLineEmpty(const char * line) {
  for(int i=0; line[i] != '\0'; i++) {
    if(isspace(line[i])==0) return 0;
  }
  return 1;
}

//return a copy of input string
//calls malloc on the new string
char* copyString(const char* stringtocopy) {
  int length_of_str = strlen(stringtocopy);
  char * newString=malloc(sizeof(char)*length_of_str+1);
  for(int i=0; i < length_of_str; i++) {
    newString[i]=stringtocopy[i];
  }
  newString[length_of_str]='\0';
  return newString;
}

//extracts a string encloses by delimiter chars
//returns NULL if not delimiter is found
//make sure to free returned string after use
//toTrimString == 1 --> will remove all whitespace from returned string
//toTrimString == 0 --> will NOT remove all whitespace from returned string
char* getStringFromDelimiter(char *line, char opening_delimiter, char closing_delimiter, int toTrimString) {
  int str_len=0; //records the length of the delimited string
  char *line_ptr=line; //keeps track of when the delimited string starts
  boolean asmetDelimiterMark=FALSE;

  for(int i=0; line[i] != closing_delimiter || asmetDelimiterMark==FALSE; i++) {

    if(line[i] == '\0') return NULL;
    if(asmetDelimiterMark == TRUE) str_len++;
    //if delimiter as never closed
    if(line[i] == opening_delimiter && asmetDelimiterMark==FALSE) {
      asmetDelimiterMark=TRUE;
      line_ptr++;
    } else if(asmetDelimiterMark == FALSE) {
      line_ptr++;
    }
  }
  //allocates memory for string
  char* parsed_str=malloc(sizeof(char)*str_len+1);
  int parsed_str_count=0;
  for(int i=0; i < str_len; i++) {
    if(toTrimString == 1 && isspace(line_ptr[i]) != 0) continue;
    parsed_str[parsed_str_count]=line_ptr[i];
    parsed_str_count++;
  }
  parsed_str[str_len]='\0';
  return parsed_str;
}

//This function gets the string from a delimiter, 
//but it will get the string that corresponds to that scope 
//For example "((123)(123))" input will return (123)(123)
//will return NULL if string is not properly scoped stacked
//this function will ignore trailing characters
//Checktrailing_WhiteSpace = 1 ----> will make there is only white before opening_delimiter
//Checktrailing_WhiteSpace = 0 ----> will not check for the latter
//opening and closing delimiter cannot be equal
//this function will return NULL if closing_delimiter comes before opening delimiter
char* getScopedDelimitedString(char* line, char opening_delimiter, char closing_delimiter, int Checktrailing_WhiteSpace) {
  if(opening_delimiter==closing_delimiter) return NULL;

  int stack_count=0;
  boolean as_met_opening_delimiter=FALSE;
  int str_len=0;
  int start_index=0;
  for(int i=0; line[i] != '\0'; i++) {
    
    //if we found our delimited string    
    if(stack_count == 0 && as_met_opening_delimiter == TRUE) break;
    
    //checks if closing delimiter happens before our opening delimiter
    //will return NULL
    if(as_met_opening_delimiter == FALSE && line[i] == closing_delimiter) return NULL;
    
    //if we meet opening delimiter
    if(line[i] == opening_delimiter) {
      if(as_met_opening_delimiter == FALSE) {
        start_index=i+1;
         as_met_opening_delimiter=TRUE;
        stack_count++;
        continue;
      } 
      str_len++;
      as_met_opening_delimiter=TRUE;
      stack_count++;

    //we check if have met an illegal char
    //depending on Checktrailing_WhiteSpace parameter 
    } else if(Checktrailing_WhiteSpace == 1 && 
      as_met_opening_delimiter == FALSE && 
      isspace(line[i]) == 0) {
        return NULL; 

    //if we meet closing delimiter
    } else if(line[i] == closing_delimiter) {
      stack_count--;
      if(stack_count > 0) str_len++;

    //otherwise its simply some char inside the delimiters
    } else if(as_met_opening_delimiter == TRUE) {
      str_len++;
    }
  }
  //means delimiters were never closed or we never met a opening delimiter
  if(stack_count > 0 || as_met_opening_delimiter ==FALSE) return NULL;

  //we malloc string and return it
  char* delimited_string = malloc(sizeof(char)*str_len+1);
  for(int i=0; i<str_len; i++) 
    delimited_string[i]=line[start_index+i];
  delimited_string[str_len]='\0';
  
  return delimited_string;
}

//this function gets the substring delimited by start and end (included)
//will return NULL if bounds are out of bounds
//start and end should not be negative 
char* getSubString(char* str, int start, int end) {
  //if the bounds are the same, then the substring is length 1
  if(start == end) {
    char* new_str= malloc(sizeof(char)+1);
    new_str[start]=str[start];
    new_str[1]='\0';
    return new_str;
  }
  //invalid bounds
  if(start < 0 || end < 0) return NULL;

  //gets the length of the string
  int str_len=0;
  for(int i=start; i <= end; i++) {
    if(str[i] == '\0') break;
    str_len++;
  }
  
  //mallocs and sets string
  char* string = malloc(sizeof(char) * str_len+1);
  for(int i=0; i < str_len; i++) 
    string[i]=str[start+i];

  string[str_len]='\0';
  return string;

}

