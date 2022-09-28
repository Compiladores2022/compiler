#include <stdio.h>
#include <stdlib.h>
#include "asm-gen.h"
#include "../list/list.h"
#include "../instruction/instruction.h"

char* create_mov_instruction(instruction_t* instruction) {
    char* mov = (char*) malloc(100 * sizeof(char));
    if (instruction->s1->flag == BASIC_F) {
        sprintf(mov, "\tmovl    $%d, %d(%%rbp)", instruction->s1->value, instruction->s3->offset);
    } else if (instruction->s1->flag == OP_F) {
        sprintf(mov, "\tmovl    %d(%%rbp), %%edx", instruction->s1->offset);
        sprintf(mov, "%s\n\tmovl    %%edx, %d(%%rbp)", mov, instruction->s3->offset);
    } else {
        printf("Error while attempting to create the asm file");
    }
    return mov;
}

char* get_mov_operand(symbol_t* s, char* reg) {
    char* mov = (char*) malloc(100 * sizeof(char));
    if (s->flag == BASIC_F) {
        sprintf(mov, "\tmovl    $%d, %%%s", s->value, reg);
    } else if (s->flag == OP_F || s->flag == ID_F) {
        sprintf(mov, "\tmovl    %d(%%rbp), %%%s", s->offset, reg);
    }
    return mov;
}

char* create_add_instruction(instruction_t* instruction) {
    char* mov_edx = get_mov_operand(instruction->s1, "edx");
    char* mov_eax = get_mov_operand(instruction->s2, "eax");
    char* add = (char*) malloc(75 * sizeof(char));
    char* mov_res = (char*) malloc(100 * sizeof(char)); 
    sprintf(add, "%s\n%s\n\taddl    %%edx, %%eax", mov_edx, mov_eax);
    sprintf(mov_res, "%s\n\tmovl    %%eax, %d(%%rbp)", add, instruction->s3->offset);
    return mov_res;
}

char* create_sub_instruction(instruction_t* instruction) {
    char* mov_edx = get_mov_operand(instruction->s1, "edx");
    char* mov_eax = get_mov_operand(instruction->s2, "eax");
    char* sub = (char*) malloc(75 * sizeof(char));
    char* mov_res = (char*) malloc(100 * sizeof(char));
    sprintf(sub, "%s\n%s\n\tsubl    %%edx, %%eax", mov_edx, mov_eax);
    sprintf(mov_res, "%s\n\tmovl    %%eax, %d(%%rbp)", sub, instruction->s3->offset);
    return mov_res;
}

char* create_mul_instruction(instruction_t* instruction) {
    char* mov_edx = get_mov_operand(instruction->s1, "edx");
    char* mov_eax = get_mov_operand(instruction->s2, "eax");
    char* mul = (char*) malloc(75 * sizeof(char));
    char* mov_res = (char*) malloc(100 * sizeof(char));
    sprintf(mul, "%s\n%s\n\timull   %%edx, %%eax", mov_edx, mov_eax);
    sprintf(mov_res, "%s\n\tmovl    %%eax, %d(%%rbp)", mul, instruction->s3->offset);
    return mov_res;
}

char* create_and_instruction(instruction_t* instruction) {
    char* mov_bl = get_mov_operand(instruction->s1, "edx");
    char* mov_al = get_mov_operand(instruction->s2, "eax");
    char* and = (char*) malloc(75 * sizeof(char));
    char* mov_res = (char*) malloc(100 * sizeof(char));
    sprintf(and, "%s\n%s\n\tandl    %%edx, %%eax", mov_bl, mov_al);
    sprintf(mov_res, "%s\n\tmovl    %%eax, %d(%%rbp)", and, instruction->s3->offset);
    return mov_res;
}

char* create_or_instruction(instruction_t* instruction) {
    char* mov_bl = get_mov_operand(instruction->s1, "edx");
    char* mov_al = get_mov_operand(instruction->s2, "eax");
    char* or = (char*) malloc(75 * sizeof(char));
    char* mov_res = (char*) malloc(100 * sizeof(char));
    sprintf(or, "%s\n%s\n\torl     %%edx, %%eax", mov_bl, mov_al);
    sprintf(mov_res, "%s\n\tmovl    %%eax, %d(%%rbp)", or, instruction->s3->offset);
    return mov_res;
}

char* create_ret_instruction(instruction_t* instruction) {
    char* mov_eax = (char*) malloc(100 * sizeof(char));
    sprintf(mov_eax, "\tmovl    %d(%%rbp), %%eax\n\tmovl    %%eax, %%edi\n\tcall    print", instruction->s3->offset);
    return mov_eax;
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
        "\tpushq   %rbp\n"
        "\tmovq    %rsp, %rbp";
}

char* epilogue() {
    return
        "\tmovq    %rsp, %rbp\n"
        "\tpopq    %rbp\n"
        "\tret";
}

void create_asm(char* filename, list_t* instruction_seq) {
    FILE* f = fopen(filename, "w+");
    if (f == NULL) {
        printf("Error while attempting to create the asm file\n");
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
