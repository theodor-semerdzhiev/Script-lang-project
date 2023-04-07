#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef enum{FALSE=0, TRUE=1} boolean;

char** parseLine(char* str, int num_of_tokens, int length_str);
int skipWhiteSpace(char* tmp_ptr);
int getToken(char* start);
char* mallocString(char* str, int length);
int* getLineCounts(char* str);

//Returns an array of strings corresponding to each token in the input string 
//Make sure to call freeArrayOfString(char**, int) to free returned char**
char** parseLine(char* str, int num_of_tokens, int length_str) {
  if(num_of_tokens == 0 || length_str == 0) return NULL;
  char *tmp_ptr=str;
  // skipWhiteSpace(tmp_ptr);
  char ** parsed_line=malloc(sizeof(char*)*num_of_tokens+1);
  int count=0;
  while(count < num_of_tokens) {
    tmp_ptr+=skipWhiteSpace(tmp_ptr);
    int token_length=getToken(tmp_ptr);
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
int skipWhiteSpace(char * tmp_ptr){
  if(isspace(tmp_ptr[0]) != 0) {
    int i=0;
    for(i=0; isspace(tmp_ptr[i]) != 0; i++) {}
    return i;
  }
  return 0;
}

//gets the amount of char until a whitespace character or a null character
int getToken(char* start) {
  int token_len=0;
  for(int i=0; start[i] != '\0' && isspace(start[i]) == 0; i++) {
    token_len++;
  } 
  return token_len;
}

//takes an initial index str and creates a substring of length int length
char* mallocString(char* str,int length) {
  char *return_str=malloc(sizeof(char)*length+1);
  for(int i=0; i < length; i++) return_str[i]=str[i];
  return_str[length]='\0';
  return return_str;
}

// return the number of tokens and the length of the string in an array respectively
// [# of tokens, # length of str] 
// length of str does not include the null char
int* getLineCounts(char* str) {
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
char* getNthToken(char* str, int position) {  
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
int isLineEmpty(char * line) {
  for(int i=0; line[i] != '\0'; i++) {
    if(isspace(line[i])==0) return 0;
  }
  return 1;
}


