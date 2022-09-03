CC = gcc

LIST := $(shell find src/libs/list -name "*.c" -not -name "*test*")
LIST_TEST := $(shell find src/libs/list -name "*.c")

STACK := $(shell find src/libs/stack -name "*.c" -not -name "*test*")
STACK_TEST := $(shell find src/libs/stack -name "*.c")

TREE := $(shell find src/libs/tree -name "*.c" -not -name "*test*")
TREE_TEST := $(shell find src/libs/tree -name "*.c")

SYMBOL := $(shTEST := $(shell find src/symbol-list -name "*.c")

SYMBOL_LIST := $(shell find src/symbol-list -name "*.c" -not -name "*test*")
SYMBOL_LIST_TEST := $(shell find src/symbol-list -name "*.c")

SYMBOL_TABLE := $(shell find src/symbol-table -name "*.c" -not -name "*test*")
SYMBOL_TABLE_TEST := $(shell find src/symbol-table -name "*.c")

SYNTAX_TREE := $(shell find src/syntax-tree -name "*.c" -not -name "*test*")
SYNTAX_TREE_TEST := $(shell find src/syntax-tree -name "*.c")

# INFRASTRUCTURE

TARGETS := list stack symbol_list symbol_table syntax_tree

all: $(TARGETS)

list: $(LIST_TEST)
tree: $(TREE_TEST)
stack: $(STACK_TEST) $(LIST)
symbol_list: $(SYMBOL_LIST_TEST) $(LIST) $(SYMBOL)
symbol_table: $(SYMBOL_TABLE_TEST) $(SYMBOL_LIST) $(SYMBOL) $(STACK) $(LIST)
syntax_tree: $(SYNTAX_TREE_TEST) $(SYMBOL) $(TREE)

$(TARGETS):
	$(CC) -o $@ $^

# COMPILER

DEPS := lex.yy.c parser.tab.c $(SYMBOL_TABLE) $(SYMBOL_LIST) $(SYMBOL) $(STACK) $(LIST)

LEXER := src/lexer.l
PARSER := src/parser.y

lex.yy.c: $(LEXER)
	flex $^

parser.tab.c parser.tab.h: $(PARSER)
	bison -d $^

compiler: $(DEPS)
	$(CC) -o npc $^

# PHONY

.PHONY: clean

clean:
	rm lex.yy.c npc parser.tab.c parser.tab.h
