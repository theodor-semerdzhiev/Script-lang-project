#ifndef KEYWORD_TABLE_H
#define KEYWORD_TABLE_H

#include "../syntax_tree/syntax_tree.h"
#include <stdlib.h>
#include <string.h>
#include "../utils/lineparsing.h"


extern struct Keyword_Table *Keyword_Table_;
void createKeywordTable();
COMMAND Keyword_Hashmap_get(const char* firstTokenOfLine);
#endif 