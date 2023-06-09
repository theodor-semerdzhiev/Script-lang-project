#ifndef LET_PARSER_H
#define LET_PARSER_H

#include "../../utils/lineparsing.h"
#include "../../types/types.h"
#include "../syntax_tree.h"

typedef struct{
  char* var_name;
  TYPE data_type;
  Variable* var;
} let_node_instruction;


PARSER_EXIT_CODE create_let_instruction(CommandList *list, char* line, int lineNumber);


#endif
