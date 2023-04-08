#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "./lineparsing.h"
#include "./syntax_analysis/let_parser.h"

typedef enum{FALSE=0, TRUE=1} boolean;

static PARSER_EXIT_CODE InitializeCommand(CommandList* syntaxtree, char *buffer);
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
  char line_ptr=fgetc(fileStream);
  char buffer[3000];
  boolean metWhiteSpace=TRUE;

  while(line_ptr != EOF) {
    //normalizes lines by removing excess whtespace
    while(line_ptr != '\n' && line_ptr != EOF) {
      if(isspace(line_ptr) == 0) {
        buffer[line_len]=line_ptr;
        metWhiteSpace=FALSE;
        line_len++;
      } else if (metWhiteSpace == FALSE) {
        buffer[line_len]=line_ptr;
        metWhiteSpace=TRUE;
        line_len++;
      } 
      line_ptr = getc(fileStream);
    }
    line_ptr=fgetc(fileStream);
    if(line_len==0) continue;
    buffer[line_len]='\0';

    if(isLineEmpty(buffer) == 1) continue; //if line is just whitespace

    // after this we will call fucntion that takes
    PARSER_EXIT_CODE line_parser_exit_code = InitializeCommand(instruc_list,buffer);

    if(line_parser_exit_code != CLEAN_EXIT)  {
      return line_parser_exit_code;
    }
    // addCommmand(instruc_list, instruc);

    if (line_ptr == EOF) break; 
    line_len=0; 
  }
  fclose(fileStream);
  return CLEAN_EXIT;
}

//might need to replaced eventually with a hash
//for the time being it works
//dont forget to have function calls to create void* to instruction node with (*void) cast
static PARSER_EXIT_CODE InitializeCommand(CommandList *syntaxtree, char *buffer) {
  char *first_token = getNthToken(buffer,1);
  if (strcmp(first_token, "let") == 0) {
    free(first_token);
    return create_let_instruction(syntaxtree,buffer);
  } else if (strcmp(first_token, "set") == 0) {
      return SET;
  } else if (strcmp(first_token, "shout") == 0) {
      return SHOUT;
  } else if (strcmp(first_token, "if") == 0) {
      return IF;
  } else if (strcmp(first_token, "end") == 0) {
      return END;
  } else if (strcmp(first_token, "for") == 0) {
      return FOR;
  } else if (strcmp(first_token, "while") == 0) {
      return WHILE;
  } else if (strcmp(first_token, "continue") == 0) {
      return CONTINUE;
  } else if (strcmp(first_token, "break") == 0) {
      return BREAK;
  } else if (strcmp(first_token, "do") == 0) {
      return DO;
  } else if (strcmp(first_token, "done") == 0) {
      return DONE;
  } else if (strcmp(first_token, "exit") == 0) {
      return EXIT;
  } else {
      return INVALID; // return an invalid command value to indicate an error
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
  parse_syntax_tree(instruc, "test.txt");

  return 0;
}