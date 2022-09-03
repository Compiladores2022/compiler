CC = gcc

LIST := $(shell find src/libs/list -name "*.c" -not -name "*test*")
LIST_TEST := $(shell find src/libs/list -name "*.c")

STACK := $(shell find src/libs/stack -name "*.c" -not -name "*test*")
STACK_TEST := $(shell find src/libs/stack -name "*.c")

TREE := $(shell find src/libs/tree -name "*.c" -not -name "*test*")
TREE_TEST := $(shell find src/libs/tree -name "*.c")

SYMBOL := $(shell find src/symbol -name "*.c")

SYMBOL_LIST := $(shell find src/symbol-list -name "*.c" -not -name "*test*")
SYMBOL_LIST_TEST := $(shell find src/symbol-list -name "*.c")

SYMBOL_TABLE := $(shell find src/symbol-table -name "*.c" -not -name "*test*")
SYMBOL_TABLE_TEST := $(shell find src/symbol-table -name "*.c")

SYNTAX_TREE := $(shell find src/syntax-tree -name "*.c" -not -name "*test*")
SYNTAX_TREE_TEST := $(shell find src/syntax-tree -name "*.c")

all: list stack symbol_list symbol_table syntax-tree

list: $(LIST_TEST)
	$(CC) -o list-test $^

tree: $(TREE_TEST)
	$(CC) -o stack-test  $^

stack: $(STACK_TEST) $(LIST)
	$(CC) -o stack-test  $^

symbol_list: $(SYMBOL_LIST_TEST) $(LIST) $(SYMBOL)
	$(CC) -o symbol-list-test  $^

symbol_table: $(SYMBOL_TABLE_TEST) $(SYMBOL_LIST) $(SYMBOL) $(STACK) $(LIST)
	$(CC) -o symbol-table-test $^

syntax_tree: $(SYNTAX_TREE_TEST) $(SYMBOL) $(TREE)
	$(CC) -o syntax-tree-test $^
