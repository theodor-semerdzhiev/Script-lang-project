#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../types/types.h"
#include "../syntax_tree/lineparsing.h"

#define DEFAULT_SIZE 75 //initial size of hashtable

//Node for the LinkedList
struct Node {
  char* var_name;
  TYPE type;
  struct Node* next;
  void* data;
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
void addVariable_to_Table(char* variable_name, void* data, TYPE type_of_var);
void InitializeVariableTable();
void removeVariable_from_Table(char *variable_name);
int containsVariable(const char* varname);
void **getVariableValue(const char *varname, TYPE type);
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
    if(strcmp(ptr->var_name,variable_name) == 0) {
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
    if(strcmp(ptr->var_name,variable_name)==0) {
      ptr->type=type_of_var;
      free(ptr->data);
      ptr->data=data;
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
void addVariable_to_Table(char* variable_name, void* data, TYPE type_of_var) {
  LinkedList *list = Variable_Table_->table[hash(variable_name)];
  if(replaceNode(list,variable_name,data,type_of_var) != NULL) //if varriable name already in list, we simply modify node
    return;
  struct Node* node = malloc(sizeof(struct Node));
  node->data=data;
  node->next=NULL;
  node->var_name=variable_name;
  node->type=type_of_var;
  addtoList(list,node);
  Variable_Table_->size++;
}

//creates the hashtable
void InitializeVariableTable() {
  LinkedList **list = malloc(sizeof(LinkedList*)*DEFAULT_SIZE);
  for(int i=0; i < DEFAULT_SIZE; i++) {
    list[i] = malloc(sizeof(LinkedList));
    list[i]->head=NULL;
    list[i]->tail=NULL;
  }
  if(Variable_Table_ == NULL) Variable_Table_=malloc(sizeof(Variable_Table));
  
  Variable_Table_->table=list;
  Variable_Table_->length=DEFAULT_SIZE;
  Variable_Table_->size=0;
}

//removes node that maps to variable_name from list
//does nothing if not in list
void removeVariable_from_Table(char *variable_name) {
  LinkedList *list = Variable_Table_->table[hash(variable_name)];
  if(list->head == NULL) 
    return;

  struct Node* tmp = list->head;
  if(strcmp(tmp->var_name,variable_name) == 0) {
    struct Node* next_node=tmp->next;
    free(tmp->var_name);
    free(tmp->data);
    free(tmp);
    list->head=next_node;
    if(next_node == NULL || next_node->next == NULL) {
      list->tail=next_node;
    }
    Variable_Table_->size--;
    return;
  }
  struct Node* prev = list->head;
  tmp=tmp->next;
  while(tmp != NULL) {
    if(strcmp(tmp->var_name,variable_name) == 0) {
      prev->next=tmp->next;
      if(tmp->next == NULL) {
        list->tail=prev;
      }
      free(tmp->var_name);
      free(tmp->data);
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

//gets the value mapped to varname
//returns an array [enum TYPE*, void* data]
//make sure to cast *void accordingly
//returns NULL if varname is not in hashtable
//make sure to free the returned array
void **getVariableValue(const char *varname, TYPE type) {
  struct Node* node= findVariableNode(Variable_Table_->table[hash(varname)],varname);
  if(node == NULL) return NULL;
  void** return_arr = malloc(sizeof(void*)*2);
  return_arr[0] = &(node->type);
  return_arr[1] = node->data;
  return return_arr;
}

//hash function
static unsigned int hash(const char* var_name) {
  unsigned int hash = 13;
    int c;
    while ((c = *var_name++))
        hash = ((hash << 13)) + c; 
    return hash % DEFAULT_SIZE;
}

//used for debugging and testing purposes
static inline void printList(LinkedList *list) {
  struct Node* ptr=list->head;
  while(ptr != NULL) {
    printf("(var: %s,",ptr->var_name);
    switch(ptr->type) {
      case INTEGER:
        printf("TYPE: int, value = %d) --> ", *(int*) ptr->data);
        break;
      case STRING:
        printf("TYPE: String, value = '%s') --> ", ptr->data);
        break;
      case DOUBLE:
        printf("TYPE: double, value = %f) --> ", *(double*) ptr->data);
        break;
    }
    ptr=ptr->next;
  }
  printf("NULL\n");
}

//used for debugging purposes
static inline void printTable() {
  for(int i =0; i< DEFAULT_SIZE; i++) {
    printList(Variable_Table_->table[i]);
  }
}

