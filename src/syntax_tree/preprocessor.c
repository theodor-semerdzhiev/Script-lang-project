#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "./lineparsing.h"

//typedef enum{FALSE=0, TRUE=1} boolean;

typedef enum {
  CLEAN_EXIT,
  UNDEFINED_KEYWORD,
  INVALID_VAR,
  INVALID_SYNTAX,
  INVALID_ASSIGNMENT,
  INVALID_FILENAME
} PARSER_EXIT_CODE; 

typedef enum {
  LET,
  SET,
  SHOUT,
  IF,
  END,
  FOR,
  WHILE,
  CONTINUE,
  BREAK,
  DO,
  DONE,
  EXIT
  //,FUNC
} COMMAND;

struct Instruction {
  char** raw_data;
  int length;
  COMMAND command;
  struct Instruction *next;
  void* command_data; //will be a pointer to the command struct
}; 

typedef struct {
  int instr_count;
  struct Instruction *head; 
  struct Instruction *tail;
} CommandList;



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

  while(line_ptr != EOF) {
    while(line_ptr != '\n' && line_ptr != EOF) {
      buffer[line_len]=line_ptr;
      line_ptr = getc(fileStream);
      line_len++;
    }
    line_ptr=fgetc(fileStream);
    if(line_len==0)continue;
    buffer[line_len]='\0';
    int* str_info=getLineCounts(buffer); //size 2
    if(str_info[0] == 0 || str_info[1] == 1) continue; // if lines is empty (just whitespace)


    // after this we will call fucntion that takes the 




    char** raw_data = parseLine(buffer,str_info[0],str_info[1]);

    struct Instruction *instruc = malloc(sizeof(struct Instruction));
    instruc->command=LET;
    instruc->length=str_info[0];
    instruc->next=NULL;
    instruc->raw_data=raw_data;

    addCommmand(instruc_list, instruc);

    free(str_info);
    if (line_ptr == EOF) break; 
    line_len=0; 
  }
  fclose(fileStream);
  return CLEAN_EXIT;
}

//might need to replaced eventually with a hash
//for the time being it works
COMMAND getCommand(char *first_token){
  if (strcmp(first_token, "let") == 0) {
      return LET;
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
      return NULL;
  }
}



int main(int argc, char *argv[]) {
  char *c = "231213 123  123   123  231 1323 123 123          ";
  int *arr = getLineCounts(c);  
  printf("%d %d\n", arr[0], arr[1]);
  // free(arr);
  char ** line = parseLine(c,arr[0],arr[1]);
  // for(int i=0; i < arr[0]; i++) printf("%s ",line[i]);

  CommandList *instruc = create_Command_list();
  parse_syntax_tree(instruc, "test.txt");
  struct Instruction *tmp = instruc->head;

  while(tmp != NULL) {
    for(int i=0; i < tmp->length; i++) printf("%s ",tmp->raw_data[i]);
    printf("\n");
    tmp=tmp->next;
  }
  printf("\n%s\n",getNthToken(c,1));
  return 0;
}