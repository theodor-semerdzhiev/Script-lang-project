#ifndef VARIABLE_TABLE_H
#define VARIABLE_TABLE_H

#include "../types/types.h"

extern struct Variable_Table *Variable_Table_;

void addVariable_to_VarTable(Variable *new_var);
void InitializeVariableTable(int initialSize);
void removeVariable_from_VarTable(char *variable_name);
int containsVariable(const char* varname);
Variable *getVariable(const char *varname);
void clearVarTable();
void printTable();
#endif