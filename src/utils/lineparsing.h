#ifndef PARSELINE_H
#define PARSELINE_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

char** tokenizeString(char* str, int num_of_tokens, int length_str);
int getCharsBeforeWhiteSpace(const char * tmp_ptr);
int getTokenLength(const char* start);
char* mallocString(const char* str, unsigned int length);
int* getLineCounts(const char* str);
int isLineEmpty(const char * line);
void freeArrayOfStrings(char** str_arr);
char* getNthToken(const char* str, int position);
char* copyString(const char* stringtocopy);
char* getStringFromDelimiter(char *line, char opening_delimiter, char closing_delimiter, int toTrimString);
char* getScopedDelimitedString(char* line, char opening_delimiter, char closing_delimiter, int Checktrailing_WhiteSpace);
char* getSubString(char* str, int start, int end);
#endif 