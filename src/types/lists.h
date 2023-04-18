#ifndef LIST_H
#define LIST_H

#include "types.h"

List* createList(int initialSize);
void append(List* list, Variable* var);
Variable* pop(List* list, int index);
Variable* get(List* list, int index);
void list_free(List* list);
void list_clear(List* list);

#endif
