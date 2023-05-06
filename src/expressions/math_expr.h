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

//this will represent a node our expression tree
struct prefix_node {
  Operation op;
  TYPE type; // will only be checked if op is NONE
  
  /*The type enum will determine if this a variable stored in the variable table or not*/
  Variable* data;
  struct prefix_node *left;
  struct prefix_node *right;
};

//expression Tree struct
typedef struct {
  struct prefix_node *head;
} Prefix_Tree;


Prefix_Tree* parse_Arithmetic_exp(char* expression);
int isArithmeticExprValid(char* expression);
Variable* EvaluateExpression(struct prefix_node* head);
