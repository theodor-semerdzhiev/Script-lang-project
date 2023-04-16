#ifndef TYPE_PARSER_H
#define TYPE_PARSER_H

#include "types.h"
#include "../syntax_tree/lineparsing.h"
#include <string.h>

TYPE getTypeHint(const char* line);
char* getStringFromDelimiter_withTrailingWhitespace(char *line, char delimiter);
int* getInteger(const char* line);
double* getDouble(const char *line);
int checkAssignmentSyntax(const char* line, const char* assignment);
String* getVariableName(char* line, int checkTrailingWhitespace);
#endif