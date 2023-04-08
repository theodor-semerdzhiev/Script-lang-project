#ifndef LET_NODE_INSTRUCTION_H
#define LET_NODE_INSTRUCTION_H

#include "../lineparsing.h"
#include "../../types/types.h"
#include "../syntax_tree.h"

typedef struct {
  char* var_name;
  TYPE data_type;
  void* data;
} let_node_instruction;

PARSER_EXIT_CODE create_let_instruction(CommandList *list, char* line);


#endif
