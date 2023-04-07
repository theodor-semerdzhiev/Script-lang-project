#ifndef PARSELINE_H
#define PARSELINE_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

char** parseLine(char* str, int num_of_tokens, int length_str);
int skipWhiteSpace(char* tmp_ptr);
int getToken(char* start);
char* mallocString(char* str, int length);
int* getLineCounts(char* str);
char* getNthToken(char* str, int position);  
int isLineEmpty(char * line);
void freeArrayOfStrings(char** str_arr);
#endif 