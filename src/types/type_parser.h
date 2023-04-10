#ifndef TYPE_PARSER_H
#define TYPE_PARSER_H

#include "types.h"
#include "../syntax_tree/lineparsing.h"
#include <string.h>

typedef enum{FALSE=0, TRUE=1} boolean;

TYPE getVariableType(const char* line);
char* getStringFromQuotationMarks(char *line);
int* getInteger(const char* line);
double* getDouble(const char *line);

#endif