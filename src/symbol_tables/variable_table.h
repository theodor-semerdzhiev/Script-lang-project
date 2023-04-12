#ifndef VARIABLE_TABLE_H
#define VARIABLE_TABLE_H

#include "../variables/types.h"

extern struct Variable_Table *Variable_Table_;

void addVariable_to_Table(char* variable_name, void* data, TYPE type_of_var);
void InitializeVariableTable(int initialSize);
void removeVariable_from_Table(char *variable_name);
int containsVariable(const char* varname);
Variable *getVariable(const char *varname);

#endif