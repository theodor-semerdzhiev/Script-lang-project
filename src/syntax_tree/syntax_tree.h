#ifndef SYNTAX_TREE_H
#define SYNTAX_TREE_H

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
    INVALID_FILENAME,
    INVALID_TYPE,
    UNCLOSED_DELIMITER
} PARSER_EXIT_CODE;

typedef enum {
    LET,
    SET,
    SHOUT,
    IF,
    THEN,
    ENDIF,
    FOR,
    WHILE,
    DO,
    DONE,
    CONTINUE,
    BREAK,
    FUNC,
    RUN,
    END,
    RETURN,
    EXIT,
    COMMENT,
    INVALID
} COMMAND;

struct Instruction {
    COMMAND command;
    struct Instruction *next;
    void *command_data; // will be a pointer to the command struct
};

typedef struct {
    int instr_count;
    struct Instruction *head;
    struct Instruction *tail;
} CommandList;

void addCommmand(CommandList *list, struct Instruction *instruc);
CommandList *create_Command_list();
PARSER_EXIT_CODE parse_syntax_tree(CommandList *instruc_list, char *script_name);

#endif 
