#ifndef SHOUT_PARSER_H
#define SHOUT_PARSER_H

#include "../../types/types.h"
#include "../syntax_tree.h"


TYPE create_shout_instruction(CommandList *list, char* line, int lineNumber);

#endif