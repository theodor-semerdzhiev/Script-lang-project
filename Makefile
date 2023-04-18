# Compiler and flags
CC = gcc
CFLAGS = -g -Wall -Wno-unused-function

# Directories
SYNTAX_TREE_DIR = ./src/syntax_tree
UTILS_DIR = ./src/utils
SYNTAX_ANALYSIS_DIR = ./src/syntax_tree/syntax_analysis
SYMBOL_TABLES_DIR = ./src/symbol_tables
TYPES_DIR = ./src/types

# Source files
SRCS = $(SYNTAX_TREE_DIR)/syntax_tree.c \
       $(UTILS_DIR)/lineparsing.c \
       $(SYNTAX_ANALYSIS_DIR)/let_parser.c \
       $(SYMBOL_TABLES_DIR)/keyword_table.c \
       $(TYPES_DIR)/type_parser.c \
       $(TYPES_DIR)/types.c \
       $(SYMBOL_TABLES_DIR)/variable_table.c \
       $(SYNTAX_ANALYSIS_DIR)/shout_parser.c \
       $(TYPES_DIR)/lists.c \
       main.c

# Object files
OBJS = $(SRCS:.c=.o)

# Executable
TARGET = main

# Rules
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	del $(subst /,\,$(OBJS)) $(subst /,\,$(TARGET))