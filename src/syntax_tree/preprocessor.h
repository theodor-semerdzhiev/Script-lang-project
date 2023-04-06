#ifndef MY_HEADER_FILE_H
#define MY_HEADER_FILE_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

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

void addCommmand(CommandList *list, struct Instruction *instruc);
CommandList *create_Command_list();
PARSER_EXIT_CODE parse_syntax_tree(CommandList *instruc_list, char* script_name);

#endif
