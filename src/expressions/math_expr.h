#include "../types/types.h"

//will define type of operation
typedef enum{
  ADD,
  SUB,
  MULT,
  DIV,
  MOD,
  NONE
} Operation;

union value_type {
  Variable* var;
  int integer;
  double floating_point;
};

//this will represent a node our expression tree
struct prefix_node {
  Operation op;
  TYPE type; // will only be checked if op is NONE
  
  union value_type data;
  struct prefix_node *left;
  struct prefix_node *right;
};

//expression Tree struct
typedef struct {
  struct prefix_node *head;
  struct prefix_node *next;
} Prefix_Tree;


Prefix_Tree* parse_Arithmetic_exp(char* expression);
int isArithmeticExprValid(char* expression);
