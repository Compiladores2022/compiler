#include "instruction.h"
#include "instruction-sequence.h"
#include "../symbol/symbol.h"
#include "../list/list.h"
#include <stdio.h>

int main() {
    symbol_t* s1 = create_symbol();
    set_name(s1, "s1");
    symbol_t* s2 = create_symbol();
    set_name(s2, "s2");
    symbol_t* s3 = create_symbol();
    set_name(s3, "s3");
    symbol_t* s4 = create_symbol();
    set_name(s4, "s4");
    instruction_t* instruction1 = new_instruction(AND, s1, s2, s3);
    instruction_t* instruction2 = new_instruction(ADD, s2, s3, s4);

    list_t* seq = new_instruction_seq();
    add_instruction(seq, instruction1);
    add_instruction(seq, instruction2);

    node_t* cursor = seq->head->next;
    while (cursor) {
        instruction_t* instruction = (instruction_t*) cursor->value;
        printf("*****\n");
        printf("%d\n", instruction->type);
        printf("%s\n", instruction->s1->name);
        printf("%s\n", instruction->s2->name);
        printf("%s\n", instruction->s3->name);
        cursor = cursor->next;
    }

    return 0;
}
