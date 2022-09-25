#include <stdio.h>
#include <stdlib.h>
#include "asm-gen.h"
#include "../list/list.h"
#include "../instruction/instruction.h"

char* create_mov_instruction(instruction_t* instruction) {
    return "Operation MOV";
}

char* create_add_instruction(instruction_t* instruction) {
    return "Operation ADD";
}

char* create_sub_instruction(instruction_t* instruction) {
    return "Operation SUB";
}

char* create_mul_instruction(instruction_t* instruction) {
    return "Operation MUL";
}

char* create_and_instruction(instruction_t* instruction) {
    return "Operation AND";
}

char* create_or_instruction(instruction_t* instruction) {
    return "Operation OR";
}

char* create_ret_instruction(instruction_t* instruction) {
    return "Operation RET";
}

char* create_asm_instruction(instruction_t* instruction) {
    switch (instruction->type) {
        case MOV:
            return create_mov_instruction(instruction);
        case ADD:
            return create_add_instruction(instruction);
        case SUB:
            return create_sub_instruction(instruction);
        case MUL:
            return create_mul_instruction(instruction);
        case AND:
            return create_and_instruction(instruction);
        case OR:
            return create_or_instruction(instruction);
        case RET:
            return create_ret_instruction(instruction);
        default:
            printf("Invalid instruction type\n");
            exit(1);
    }
}

char* prologue() {
    return 
        "\t.globl main\n"
        "main:\n"
        "\tpushq   \%rbp\n"
        "\tmovq    \%rsp, \%rbp";
}

char* epilogue() {
    return
        "\tmovq    \%rsp, \%rbp\n"
        "\tpopq    \%rbp\n"
        "\tret";
}

void create_asm(char* filename, list_t* instruction_seq) {
    FILE* f = fopen(filename, "w+");
    if (f == NULL) {
        printf("Error while tried to create the asm file\n");
        exit(1);
    }
    
    fprintf(f, "%s\n", prologue());

    node_t* cursor = instruction_seq->head->next;
    while (cursor != NULL) {
        instruction_t* instruction = (instruction_t*) cursor->value;
        char* asm_instruction = create_asm_instruction(instruction);
        fprintf(f, "%s\n", asm_instruction);
        cursor = cursor->next;
    }

    fprintf(f, "%s\n", epilogue());

    fclose(f);
}
