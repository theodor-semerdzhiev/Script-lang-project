#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../types/types.h"
#include "../utils/lineparsing.h"

//Node for the LinkedList
struct Node {
  TYPE type;
  struct Node* next;
  Variable* var;
};

//a linkedlist used for chaining in my hashmap
typedef struct {
  struct Node* head;
  struct Node* tail;
} LinkedList;

typedef struct{
  LinkedList **table;
  int size; // number of variables stored in the table
  int length; //stores the size of the table
} Variable_Table;


static Variable_Table *Variable_Table_;

static inline void addtoList(LinkedList *list, struct Node  *list_node);
static struct Node* findVariableNode(LinkedList *list, const char* variable_name);
static unsigned int hash(const char* var_name);
static struct Node* replaceNode(LinkedList *list, char* variable_name, void* data, TYPE type_of_var);


//adds to list
static inline void addtoList(LinkedList *list, struct Node *list_node) {
  if(list->head == NULL) {
    list->head = list_node;
    list->tail = list_node;
    return;
  }
  list->tail->next = list_node;
  list->tail=list->tail->next;
}

//iterates through list and returns the node corresponding to variable_name
//if no node is found, then it returns NULL
static struct Node* findVariableNode(LinkedList *list, const char* variable_name) {
  struct Node* ptr = list->head;
  while(ptr != NULL) {
    if(strcmp(ptr->var->name,variable_name) == 0) {
      return ptr;
    } 
    ptr=ptr->next;
  }
  //will return null if variable is not found
  return NULL;
}

//iterates throught list, if proper node is found, type and data is modified
static struct Node* replaceNode(LinkedList *list, char* variable_name, void* data, TYPE type_of_var) {
  struct Node* ptr = list->head;
  while(ptr != NULL) {
    if(strcmp(ptr->var->name,variable_name)==0) {
      ptr->type=type_of_var;
      
      modifyVariable(ptr->var,variable_name,data,type_of_var);

      return ptr;
    }
    ptr=ptr->next;
  }
  return NULL;
}

//returns the data stored in the variable
//adds variable to the hashtable
//checks if variable_name is already in hashmap first
//variable_name and data MUST be malloced before function call
void addVariable_to_VarTable(char* variable_name, void* data, TYPE type_of_var) {
  LinkedList *list = Variable_Table_->table[hash(variable_name)];

  //if varriable name already in list, we simply modify node
  if(replaceNode(list,variable_name,data,type_of_var) != NULL) 
    return;
  struct Node* node = malloc(sizeof(struct Node));
  Variable *variable;
  if(type_of_var == STRING) variable=createVariableStruct(type_of_var,variable_name,data,strlen((char*)data));
  else                      variable=createVariableStruct(type_of_var,variable_name,data,0);
  node->next=NULL;
  node->type=type_of_var;
  node->var=variable;
  addtoList(list,node);
  Variable_Table_->size++;
}

//creates the hashtable
void InitializeVariableTable(int initialSize) {
  LinkedList **list = malloc(sizeof(LinkedList*)*initialSize);
  for(int i=0; i < initialSize; i++) {
    list[i] = malloc(sizeof(LinkedList));
    list[i]->head=NULL;
    list[i]->tail=NULL;
  }
  if(Variable_Table_ == NULL) Variable_Table_=malloc(sizeof(Variable_Table));
  
  Variable_Table_->table=list;
  Variable_Table_->length=initialSize;
  Variable_Table_->size=0;
}

//removes node that maps to variable_name from list
//does nothing if not in list
void removeVariable_from_VarTable(char *variable_name) {
  LinkedList *list = Variable_Table_->table[hash(variable_name)];
  if(list->head == NULL) 
    return;

  struct Node* tmp = list->head;
  struct Node* prev = NULL;
  while(tmp != NULL) {
    if(strcmp(tmp->var->name,variable_name) == 0) {
      if(prev == NULL) {
        list->head=tmp->next;
      } else {
        prev->next=tmp->next;
      }
      if(tmp->next == NULL) {
        list->tail=prev;
        prev->next=NULL;
      }
      freeVariableStruct(tmp->var);
      free(tmp);
      Variable_Table_->size--;
      return;
    }
    prev=tmp;
    tmp=tmp->next;
  }
}

//checks if a variable exists in the variable table
//will return 1 if yes
//0 if no
int containsVariable(const char* varname) {
  if(findVariableNode(Variable_Table_->table[hash(varname)],varname) != NULL) {
    return 1;
  }
  return 0;
}

//Gets the value mapped to varname and
//returns an array [enum TYPE, void* data].
//Make sure to cast *void accordingly
//returns NULL if varname is not in hashtable and
//make sure to free the returned array.
Variable *getVariable(const char *varname) {
  struct Node* node= findVariableNode(Variable_Table_->table[hash(varname)],varname);
  if(node == NULL) return NULL;
  return node->var;
}

//gets the type of the variable
//return UNKNOWN if variable is not in hashtable
TYPE getVariableType(const char* varname) {
  struct Node* node= findVariableNode(Variable_Table_->table[hash(varname)],varname);
  if(node == NULL) return UNKNOWN;
  return node->type;
}

//clears the variable from hashtable
void clearVarTable() {
  for(int i=0; i < Variable_Table_->length; i++) {
    LinkedList* list = Variable_Table_->table[i];
    if(list->head==NULL) continue;
    struct Node* ptr = list->head;
    while(ptr != NULL) {
      struct Node* tmp=ptr->next;
      freeVariableStruct(ptr->var);
      free(ptr);
      ptr=tmp;
    }
    list->head=NULL;
    list->tail=NULL;
  }
}

//hash function
static unsigned int hash(const char* var_name) {
  unsigned int hash = 13;
    int c;
    while (c = *var_name++)
        hash = (hash << 13) + c; 
    return hash % Variable_Table_->length;
}

//used for debugging and testing purposes
static void printList(LinkedList *list) {
  struct Node* ptr=list->head;
  while(ptr != NULL) {
    printf("(var: %s,",ptr->var->name);
    switch(ptr->type) {
      case INTEGER:
        printf("TYPE: int, value = %d) --> ",  ptr->var->data.integer);
        break;
      case STRING:
        printf("TYPE: String, value = '%s') --> ", ptr->var->data.str->string);
        break;
      case DOUBLE:
        printf("TYPE: double, value = %f) --> ", ptr->var->data.floatingpoint);
        break;
      case BOOL:
        printf("TYPE: boolean, value = %d) --> ", ptr->var->data.boolean);
        break;
      case ARRAY:
        break;
      case FUNCTION:
        break;
      case VAR:
        break;
      case _NULL:
        break;
      case ARITHMETIC_EXPRESSION:
        break;
      case BOOL_EXPRESSION:
        break;
      case UNKNOWN:
        break;
    }
    ptr=ptr->next;
  }
  printf("NULL\n");
}

//used for debugging purposes
static inline void printTable() {
  for(int i =0; i< Variable_Table_->length; i++) {
    printList(Variable_Table_->table[i]);
  }
}

// int main() {
//    char* string_array[50] = {
//         "Hello", "World", "C", "Programming", "is", "fun", "and", "useful", "for", "building",
//         "great", "applications", "that", "people", "can", "use", "to", "improve", "their",
//         "lives", "and", "solve", "problems", "in", "innovative", "ways", "Programming", "is",
//         "also", "a", "valuable", "skill", "for", "anyone", "looking", "to", "pursue", "a",
//         "career", "in", "technology", "or", "just", "to", "learn", "something", "new", "and",
//         "exciting"
//     };

//   InitializeVariableTable(50);
//   for(int i=0; i < 49; i++) {
//        char* name=malloc(sizeof(char)*20);
//     strcpy(name,string_array[i]);

//     if(i % 3==0) {
//       int *num=malloc(sizeof(int));
//       *num=1;
//       addVariable_to_VarTable(name,num,INTEGER);

//     } else if(i % 3 == 1) {
//       double *num=malloc(sizeof(double));
//       *num=i+0.5;
//       addVariable_to_VarTable(name,num,DOUBLE);

//     } else if(i % 3 == 2) {
//       char* var_name=malloc(sizeof(char)*30);
//       strcpy(var_name,name);
//       addVariable_to_VarTable(name,var_name,STRING);
//     }
//   }
//   // printTable();
//   // for(int i=0; i< 49; i++) {
//   //   removeVariable_from_VarTable(string_array[i]);
//   // }
//   printTable();
//   clearVarTable();
// }


