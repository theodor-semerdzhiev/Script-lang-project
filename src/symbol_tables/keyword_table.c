#include "../syntax_tree/syntax_tree.h"
#include <stdlib.h>
#include <string.h>
#include "../syntax_tree/lineparsing.h"

#define KEYWORD_COUNT 17 //we could there is 18 keywords, but INVALID is not counted
#define TABLE_SIZE 20 //how big the hash table should be 
#define MAX_KEYWORD_LENGTH 15//maximum length of a keyword

//Node for the LinkedList
struct Node{
  COMMAND keyword_enum;
  char *keyword;
  struct Node* next;
};


//a linkedlist used for chaining in my hashmap
struct LinkedList{
  struct Node* head;
  struct Node* tail;
};

//hashmap
struct Keyword_Table{
  struct LinkedList **table;
  int length;
};

//acts as a constant struct that will exsist for the duration of the program runtime
static struct Keyword_Table *Keyword_Table_;

static void addtoList(struct LinkedList *list, struct Node *keyword);
static COMMAND findKeyword(struct LinkedList *list, const char * firstTokenOfLine);
void createTable();
static void Hashmap_add(char *keyword, COMMAND identifer);
COMMAND Hashmap_get(const char* firstTokenOfLine);
unsigned long hash(const char *str);
static void printList(struct LinkedList *list);
static void printTable(struct LinkedList **list);

//adds to chaining list
static void addtoList(struct LinkedList *list, struct Node *keyword) {
  if(list->head == NULL) {
    list->head = keyword;
    list->tail = keyword;
    return;
  }
  list->tail->next=keyword;
  list->tail-list->tail->next;
}

//iterates through array, until it finds the proper key
//return the UNKNOWN enum type if it does not find it
static COMMAND findKeyword(struct LinkedList *list, const char * firstTokenOfLine) {
  struct Node* ptr = list->head;
  while(ptr != NULL) {
    if(strcmp(ptr->keyword,firstTokenOfLine) == 0) {
      return ptr->keyword_enum;
    } 
    ptr=ptr->next;
  }
  return INVALID;
} 

//mallocs hashtable and adds appropriate keys along with their enums
void createTable() {
  struct LinkedList **table=malloc(sizeof(struct LinkedList*)*TABLE_SIZE);
  for(int i=0; i< TABLE_SIZE; i++) {
    table[i]=malloc(sizeof(struct LinkedList));
    table[i]->head=NULL;
    table[i]->tail=NULL;
  }
  if(Keyword_Table_ == NULL) {
    Keyword_Table_=malloc(sizeof(struct Keyword_Table));
  }
  Keyword_Table_->length=TABLE_SIZE;
  Keyword_Table_->table=table;

  char *keywords[] = {
    "let", "set", "shout", "if", "then", "endif", "for", "while",
    "do", "done", "continue", "break", "func", "run", "end", "return","exit", "invalid"
  };
  COMMAND keyword_enums[] = {
    LET, SET, SHOUT, IF, THEN, ENDIF, FOR, WHILE,
    DO, DONE, CONTINUE, BREAK, FUNC, RUN, END, RETURN, EXIT, INVALID
  };

  for(int i=0; i < MAX_KEYWORD_LENGTH; i++) {
    Hashmap_add(copyString(keywords[i]), keyword_enums[i]);
  }
}

//adds to our hashmap
static void Hashmap_add(char *cpy_keyword,COMMAND type) {
  struct Node *node=malloc(sizeof(struct Node));
  node->keyword=cpy_keyword;
  node->keyword_enum=type;
  node->next=NULL;
  addtoList(Keyword_Table_->table[hash(cpy_keyword)],node);
}

COMMAND Hashmap_get(const char* firstTokenOfLine) {
  return findKeyword(Keyword_Table_->table[hash(firstTokenOfLine)],firstTokenOfLine);
}

//hash function, tailored for our inputs (zero collisions)
unsigned long hash(const char *str) {
    unsigned long hash = 101;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash*c) + c; 
    return hash % TABLE_SIZE;
}

//for utility and testing
static void printList(struct LinkedList *list) {
  struct Node* ptr=list->head;
  while(ptr != NULL) {
    printf("%s->",ptr->keyword);
    ptr=ptr->next;
  }
  printf("NULL\n");
}

//for utility and testing
static void printTable(struct LinkedList **list) {
  for(int i =0; i< TABLE_SIZE; i++) {
    printList(list[i]);
  }
}

