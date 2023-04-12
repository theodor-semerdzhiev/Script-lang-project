#ifndef VARIABLE_PARSER_H
#define VARIABLE_PARSER_H

#include "types.h"
#include "../syntax_tree/lineparsing.h"
#include <string.h>

typedef enum{FALSE=0, TRUE=1} boolean;

TYPE getAssignmentType(const char* line);
char* getStringFromQuotationMarks(char *line);
int* getInteger(const char* line);
double* getDouble(const char *line);
int checkAssignmentSyntax(const char* line, const char* assignment);
#endif