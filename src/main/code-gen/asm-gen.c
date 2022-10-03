#include <stdio.h>
#include <stdlib.h>
#include "asm-gen.h"
#include "../list/list.h"
#include "../instruction/instruction.h"

char* create_mov_instruction(instruction_t* instruction) {
    char* mov = (char*) malloc(100 * sizeof(char));
    if (instruction->s1->flag == BASIC_F) {
        sprintf(mov, "\tmovl    $%d, %d(%%rbp)", instruction->s1->value, instruction->s3->offset);
    } else if (instruction->s1->flag == BIN_OP_F || instruction->s1->flag == UN_OP_F) {
        sprintf(mov, "\tmovl    %d(%%rbp), %%edx", instruction->s1->offset);
        sprintf(mov, "%s\n\tmovl    %%edx, %d(%%rbp)", mov, instruction->s3->offset);
    } else {
        printf("Error while attempting to create the asm file");
    }
    return mov;
}

char* mov_operand(symbol_t* s, char* reg) {
    char* mov = (char*) malloc(100 * sizeof(char));
    if (s->flag == BASIC_F) {
        sprintf(mov, "\tmovl    $%d, %%%s", s->value, reg);
    } else if (s->flag == ID_F || s->flag == BIN_OP_F || s->flag == UN_OP_F) {
        sprintf(mov, "\tmovl    %d(%%rbp), %%%s", s->offset, reg);
    }
    return mov;
}

char* create_add_instruction(instruction_t* instruction) {
    char* mov_eax = mov_operand(instruction->s1, "eax");
    char* mov_edx = mov_operand(instruction->s2, "edx");
    char* add = (char*) malloc(75 * sizeof(char));
    char* mov_res = (char*) malloc(100 * sizeof(char)); 
    sprintf(add, "%s\n%s\n\taddl    %%edx, %%eax", mov_edx, mov_eax);
    sprintf(mov_res, "%s\n\tmovl    %%eax, %d(%%rbp)", add, instruction->s3->offset);
    return mov_res;
}

char* create_sub_instruction(instruction_t* instruction) {
    char* mov_eax = mov_operand(instruction->s1, "eax");
    char* mov_edx = mov_operand(instruction->s2, "edx");
    char* sub = (char*) malloc(75 * sizeof(char));
    char* mov_res = (char*) malloc(100 * sizeof(char));
    sprintf(sub, "%s\n%s\n\tsubl    %%edx, %%eax", mov_edx, mov_eax);
    sprintf(mov_res, "%s\n\tmovl    %%eax, %d(%%rbp)", sub, instruction->s3->offset);
    return mov_res;
}

char* create_mul_instruction(instruction_t* instruction) {
    char* mov_eax = mov_operand(instruction->s1, "eax");
    char* mov_edx = mov_operand(instruction->s2, "edx");
    char* mul = (char*) malloc(75 * sizeof(char));
    char* mov_res = (char*) malloc(100 * sizeof(char));
    sprintf(mul, "%s\n%s\n\timull   %%edx, %%eax", mov_edx, mov_eax);
    sprintf(mov_res, "%s\n\tmovl    %%eax, %d(%%rbp)", mul, instruction->s3->offset);
    return mov_res;
}

char* create_and_instruction(instruction_t* instruction) {
    char* mov_eax = mov_operand(instruction->s1, "eax");
    char* mov_edx = mov_operand(instruction->s2, "edx");
    char* and = (char*) malloc(75 * sizeof(char));
    char* mov_res = (char*) malloc(100 * sizeof(char));
    sprintf(and, "%s\n%s\n\tandl    %%edx, %%eax", mov_edx, mov_eax);
    sprintf(mov_res, "%s\n\tmovl    %%eax, %d(%%rbp)", and, instruction->s3->offset);
    return mov_res;
}

char* create_or_instruction(instruction_t* instruction) {
    char* mov_eax = mov_operand(instruction->s1, "eax");
    char* mov_edx = mov_operand(instruction->s2, "edx");
    char* or = (char*) malloc(75 * sizeof(char));
    char* mov_res = (char*) malloc(100 * sizeof(char));
    sprintf(or, "%s\n%s\n\torl     %%edx, %%eax", mov_edx, mov_eax);
    sprintf(mov_res, "%s\n\tmovl    %%eax, %d(%%rbp)", or, instruction->s3->offset);
    return mov_res;
}

char* create_eq_instruction(instruction_t* instruction) {
    char* mov_eax = mov_operand(instruction->s1, "eax");
    char* mov_edx = mov_operand(instruction->s2, "edx");
    char* eq = (char*) malloc(100 * sizeof(char));
    char* mov_res = (char*) malloc(125 * sizeof(char));
    sprintf(eq, "%s\n%s\n\tcmpl     %%edx, %%eax", mov_edx, mov_eax);
	sprintf(eq, "%s\n\tsete	%%al\n\tmovzbl  %%al, %%eax", eq);
    sprintf(mov_res, "%s\n\tmovl    %%eax, %d(%%rbp)", eq, instruction->s3->offset);
    return mov_res;
}

char* create_gt_instruction(instruction_t* instruction) {
    char* mov_eax = mov_operand(instruction->s1, "eax");
    char* mov_edx = mov_operand(instruction->s2, "edx");
    char* gt = (char*) malloc(100 * sizeof(char));
    char* mov_res = (char*) malloc(125 * sizeof(char));
    sprintf(gt, "%s\n%s\n\tcmpl     %%edx, %%eax", mov_edx, mov_eax);
	sprintf(gt, "%s\n\tsetg	%%al\n\tmovzbl  %%al, %%eax", gt);
    sprintf(mov_res, "%s\n\tmovl    %%eax, %d(%%rbp)", gt, instruction->s3->offset);
    return mov_res;
}

char* create_lt_instruction(instruction_t* instruction) {
    char* mov_eax = mov_operand(instruction->s1, "eax");
    char* mov_edx = mov_operand(instruction->s2, "edx");
    char* lt = (char*) malloc(100 * sizeof(char));
    char* mov_res = (char*) malloc(125 * sizeof(char));
    sprintf(lt, "%s\n%s\n\tcmpl     %%edx, %%eax", mov_edx, mov_eax);
	sprintf(lt, "%s\n\tsetl	%%al\n\tmovzbl  %%al, %%eax", lt);
    sprintf(mov_res, "%s\n\tmovl    %%eax, %d(%%rbp)", lt, instruction->s3->offset);
    return mov_res;
}

char* create_min_instruction(instruction_t* instruction) {
    char* mov_eax = mov_operand(instruction->s1, "eax");
    char* min = (char*) malloc(50 * sizeof(char));
    char* mov_res = (char*) malloc(75 * sizeof(char));
    sprintf(min, "%s\n\tnegl    %%eax", mov_eax);
    sprintf(mov_res, "%s\n\tmovl    %%eax, %d(%%rbp)", min, instruction->s3->offset);
    return mov_res;
}

char* create_neg_instruction(instruction_t* instruction) {
    char* mov_eax = mov_operand(instruction->s1, "eax");
    char* neg = (char*) malloc(50 * sizeof(char));
    char* mov_res = (char*) malloc(100 * sizeof(char));
    sprintf(neg, "%s\n\tcmpl    $0, %%eax", mov_eax);
	sprintf(neg, "%s\n\tsete	%%al\n\tmovzbl  %%al, %%eax", neg);
    sprintf(mov_res, "%s\n\tmovl    %%eax, %d(%%rbp)", neg, instruction->s3->offset);
    return mov_res;
}

char* create_ret_instruction(instruction_t* instruction) {
    char* mov_eax = (char*) malloc(100 * sizeof(char));
    sprintf(mov_eax, "\tmovl    %d(%%rbp), %%eax\n\tmovl    %%eax, %%edi\n\tcall    print", instruction->s3->offset);
    return mov_eax;
}

char* create_asm_instruction(instruction_t* instruction) {
    switch (instruction->type) {
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
        case EQ:
            return create_eq_instruction(instruction);
        case GT:
            return create_gt_instruction(instruction);
        case LT:
            return create_lt_instruction(instruction);
        case MIN:
            return create_min_instruction(instruction);
        case NEG:
            return create_neg_instruction(instruction);
        case MOV:
            return create_mov_instruction(instruction);
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
