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

//will represent a value (immediate)
struct Value{
  TYPE type;
  union data{
    int integer;
    double floating_point;
  };
};

//will represent a variable
struct Var{
  char* var_name;
};


//this will represent a node our expression tree
struct prefix_node {
  Operation op;

  union operation {
    struct Var;
    struct Value;
  };
  struct prefix_node *left;
  struct prefix_node *right;
};

struct evaluated_expression {
  TYPE type;
  union {
    int integer;
    double floating_point;
  };
  
};

//expression Tree struct
typedef struct {
  struct prefix_node *head;
} Prefix_Tree;