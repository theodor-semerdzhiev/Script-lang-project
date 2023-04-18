#include <string.h>
#include "../../utils/lineparsing.h"
#include "../../types/types.h"
#include "../syntax_tree.h"
#include "../../types/type_parser.h"

struct Node{
  TYPE type; // will realistically be EITHER STRING or VAR
  char* string; // will either contain some string or a variable name (determined by the TYPE enum)
  struct Node* next;
};

typedef struct {
  struct Node* head;
  struct Node* tail;
} shout_instruction;

static void addToList(shout_instruction *shout, struct Node* node);
TYPE create_shout_instruction(CommandList *list, char* line, int lineNumber);


TYPE create_shout_instruction(CommandList *list, char* line, int lineNumber) {
  line+=5;
  shout_instruction *shout_instruc=malloc(sizeof(shout_instruction));
  shout_instruc->head=NULL;
  shout_instruc->tail=NULL;

  while(*line != '\0') {
    //moves line pointer until the next declaration
    while(isspace(*line) != 0) line++; 
    String *data=NULL;
    
    //we referencing a variable
    if(*line == '$') {
      //gets the variable name
      data=getVariableName(line,0); 
      if(data == NULL) {
        free(shout_instruc);
        return INVALID_VAR;
      }
      
    //referencing a string
    } else if(*line == '"') {
      data=malloc(sizeof(String));

      //gets the string from the "" delimiters
      data->string=getStringFromDelimiter(line,'"','"',0);
      
      //if syntax invalid 
      if(data->string == NULL) {
        free(data);
        free(shout_instruc);
        return UNCLOSED_DELIMITER;
      }

    // if not $ or ", its invalid syntax
    } else {
      free(shout_instruc);
      return INVALID_SYNTAX;
    }

    struct Node* node = malloc(sizeof(struct Node));
    
    //this jumps the line pointer to whatever is after the declaration
    //also sets type of the node
    if(*line =='"') {
      node->type=STRING;
      line+=strlen(data->string)+2;
    } else {
      node->type=VAR;
      line+=data->length+1;
    }

    node->string=data->string;
    node->next=NULL;
    
    
    free(data);
    addToList(shout_instruc,node);
  }

  //creates instruction node
  struct Instruction *instruction_node = malloc(sizeof(struct Instruction));
  instruction_node->next=NULL;
  instruction_node->command = SHOUT;
  instruction_node->command_data=shout_instruc;

  addCommmand(list,instruction_node);
  return CLEAN_EXIT;
}

//adds to list
static void addToList(shout_instruction *shout, struct Node* node) {
  if(shout->head==NULL) {
    shout->head=node;
    shout->tail=node;
    return;
  }
  shout->tail->next=node;
  shout->tail=shout->tail->next;
}