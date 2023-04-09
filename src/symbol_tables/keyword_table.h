#ifndef KEYWORD_TABLE_H
#define KEYWORD_TABLE_H

#include "../syntax_tree/syntax_tree.h"
#include <stdlib.h>
#include <string.h>
#include "../syntax_tree/lineparsing.h"

extern struct Keyword_Table *Keyword_Table_;
void createTable();
COMMAND Hashmap_get(const char* firstTokenOfLine);
#endif 