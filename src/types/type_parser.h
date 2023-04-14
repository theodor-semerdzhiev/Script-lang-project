#ifndef TYPE_PARSER_H
#define TYPE_PARSER_H

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
#endif