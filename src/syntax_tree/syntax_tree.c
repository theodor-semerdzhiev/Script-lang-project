#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "./lineparsing.h"
#include "./syntax_analysis/let_parser.h"
#include "../symbol_tables/keyword_table.h"
#include "../symbol_tables/variable_table.h"

typedef enum{FALSE=0, TRUE=1} boolean;

static PARSER_EXIT_CODE InitializeCommand(CommandList* syntaxtree, char *buffer, int linenumber);
void addCommmand(CommandList *list, struct Instruction *instruc);
CommandList *create_Command_list();
PARSER_EXIT_CODE parse_syntax_tree(CommandList *instruc_list, char* script_name);

void addCommmand(CommandList *list, struct Instruction *instruc) {
  if(list->head == NULL) {
    list->head=instruc;
    list->tail=instruc;
    return;
  }
  list->tail->next=instruc;
  list->tail=instruc;
}

CommandList *create_Command_list() {
  CommandList* instruc_list=malloc(sizeof(CommandList));
  instruc_list->instr_count=0;
  instruc_list->head=NULL;
  instruc_list->tail=NULL;
  return instruc_list;
}

PARSER_EXIT_CODE parse_syntax_tree(CommandList *instruc_list, char* script_name) {
  FILE *fileStream=fopen(script_name,"r");
  if(fileStream == NULL) return INVALID_FILENAME;

  int line_len=0;
  int line_number=1;
  char line_ptr=fgetc(fileStream);
  char buffer[3000];
  boolean metWhiteSpace=TRUE;

  createKeywordTable(); //initiliazes keyword hashtable
  InitializeVariableTable(250); //initilizes variable hashtable
  while(line_ptr != EOF) {
    //trims string by removing excess whitespace
    while(line_ptr != '\n' && line_ptr != EOF) {
      if(isspace(line_ptr) == 0) {
        buffer[line_len]=line_ptr;
        metWhiteSpace=FALSE;
        line_len++;
      } else if (metWhiteSpace == FALSE) {
        buffer[line_len]=line_ptr;
        line_len++;
      } 
      line_ptr = getc(fileStream);
    }
    line_ptr=fgetc(fileStream);
    if(line_len==0) {
      line_number++;
      continue;
    }
    buffer[line_len]='\0';

    if(isLineEmpty(buffer) == 1) {
      line_number++;
      continue; //if line is just whitespace
    }
    // after this we will call fucntion that takes
    PARSER_EXIT_CODE line_parser_exit_code = InitializeCommand(instruc_list,buffer,line_number);

    if(line_parser_exit_code != CLEAN_EXIT)  {
      printf("Error at line %d: %s", line_number,buffer);
      return line_parser_exit_code;
    }
    // addCommmand(instruc_list, instruc);

    if (line_ptr == EOF) break; 
    line_len=0; 
    line_number++;
  }
  fclose(fileStream);
  return CLEAN_EXIT;
}


static PARSER_EXIT_CODE InitializeCommand(CommandList *syntaxtree, char *buffer, int lineNumber) {
  char *first_token = getNthToken(buffer,1);
  switch(Keyword_Hashmap_get(first_token)) {
    case LET:
      return create_let_instruction(syntaxtree,buffer,lineNumber);
    //TODO
    case SET:
      return CLEAN_EXIT;  
    case SHOUT:
      return CLEAN_EXIT;
    case IF:
      return CLEAN_EXIT;
    case THEN:
      return CLEAN_EXIT;
    case ENDIF:
      return CLEAN_EXIT;
    case FOR:
      return CLEAN_EXIT;
    case WHILE:
      return CLEAN_EXIT;
    case DO:
      return CLEAN_EXIT;
    case DONE:
      return CLEAN_EXIT;
    case CONTINUE:
      return CLEAN_EXIT;
    case BREAK:
      return CLEAN_EXIT;
    case FUNC:
      return CLEAN_EXIT;
    case RUN:
      return CLEAN_EXIT;
    case END:
      return CLEAN_EXIT;
    case RETURN:
      return CLEAN_EXIT;
    case EXIT:
      return CLEAN_EXIT;
    case INVALID:
      return UNDEFINED_KEYWORD;
  }
}




int main(int argc, char *argv[]) {
  char *c = "231213 123  123   123  231 1323 123 123          ";
  int *arr = getLineCounts(c);  
  //printf("%d %d\n", arr[0], arr[1]);
  // free(arr);
  char ** line = parseLine(c,arr[0],arr[1]);
  // for(int i=0; i < arr[0]; i++) printf("%s ",line[i]);

  CommandList *instruc = create_Command_list();
  parse_syntax_tree(instruc, "test_script.txt");


  return 0;
}