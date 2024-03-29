#include <stdio.h>
#include <stdlib.h>
#include "asm-gen.h"
#include "../list/list.h"
#include "../instruction/instruction.h"
#include "asm-gen-utils.h"

char* prologue(char* name) {
    char* prologue = (char*) malloc(100 * sizeof(char));
    sprintf(prologue, "\t.globl %s", name);
    sprintf(prologue, "%s\n%s:", prologue, name);
    sprintf(prologue, "%s\n\tpushq   %%rbp", prologue);
    sprintf(prologue, "%s\n\tmovq    %%rsp, %%rbp", prologue);
    return prologue;
}

char* epilogue(char* curr_proc_out_lbl) {
    char* epilogue = (char*) malloc(100 * sizeof(char));
    sprintf(epilogue, "%s:", curr_proc_out_lbl);
    sprintf(epilogue, "%s\n\tmovq    %%rbp, %%rsp", epilogue);
    sprintf(epilogue, "%s\n\tpopq    %%rbp", epilogue);
    sprintf(epilogue, "%s\n\tret", epilogue);
    return epilogue;
}

char* create_mov_instruction(instruction_t* instruction) {
    symbol_t* in = instruction->s1;
    symbol_t* out = instruction->s3;

    char* mov = (char*) malloc(200 * sizeof(char));
    sprintf(mov, "\tmovl    %s", get_asm_address(in));
    if (is_mem_ref(in) && is_mem_ref(out)) {
        sprintf(mov, "%s%s", mov, get_edx_intermediation());
    }
    sprintf(mov, "%s, %s", mov, get_asm_address(out));

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
    if (instruction->s2->flag == REG_F) {
        char* subq = (char*) malloc(25 * sizeof(char));
        sprintf(subq, "\tsubq    $%d, %%rsp", -instruction->s1->offset);
        return subq;
    }

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

char* create_div_instruction(instruction_t* instruction) {
    char* mov_eax = mov_operand(instruction->s1, "eax");
    char* mov_ebx = mov_operand(instruction->s2, "ebx");
    char* div = (char*) malloc(75 * sizeof(char));
    char* mov_res = (char*) malloc(100 * sizeof(char));
    sprintf(div, "%s\n%s\n\tcltd\n\tidivl   %%ebx", mov_eax, mov_ebx);
    sprintf(mov_res, "%s\n\tmovl    %%eax, %d(%%rbp)", div, instruction->s3->offset);
    return mov_res;
}

char* create_mod_instruction(instruction_t* instruction) {
    char* mov_eax = mov_operand(instruction->s1, "eax");
    char* mov_ebx = mov_operand(instruction->s2, "ebx");
    char* mod = (char*) malloc(75 * sizeof(char));
    char* mov_res = (char*) malloc(100 * sizeof(char));
    sprintf(mod, "%s\n%s\n\tcltd\n\tidivl   %%ebx", mov_eax, mov_ebx);
    sprintf(mov_res, "%s\n\tmovl    %%edx, %d(%%rbp)", mod, instruction->s3->offset);
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
    symbol_t* symbol_to_return = instruction->s3;

    char* ret = (char*) malloc(150 * sizeof(char));
    if (!symbol_to_return) { // return void
        sprintf(ret, "\tmovl    $0, %%eax");
    } else {
        sprintf(ret, "\tmovl    %s, %%eax", get_asm_address(symbol_to_return));
    }
    sprintf(ret, "%s\n\tjmp     %s", ret, instruction->s1->name);
    return ret;
}

char* create_jmp_instruction(instruction_t* instruction) {
    char* jmp = (char*) malloc(100 * sizeof(char));
    char* label_name = instruction->s3->name;
    sprintf(jmp, "\tjmp     %s", label_name);
    return jmp;
}

char* create_je_instruction(instruction_t* instruction) {
    char* je = (char*) malloc(100 * sizeof(char));
    char* label_name = instruction->s3->name;
    sprintf(je, "\tcmpl    $0, %d(%%rbp)\n\tje      %s", instruction->s1->offset, label_name);
    return je;
}

char* create_jne_instruction(instruction_t* instruction) {
    char* jne = (char*) malloc(100 * sizeof(char));
    char* label_name = instruction->s3->name;
    sprintf(jne, "\tcmpl    $0, %d(%%rbp)\n\tjne     %s", instruction->s1->offset, label_name);
    return jne;
}

char* create_lbl_instruction(instruction_t* instruction) {
    char* lbl = (char*) malloc(20 * sizeof(char));
    char* label_name = instruction->s3->name;
    sprintf(lbl, "%s:", label_name);
    return lbl;
}

char* create_call_instruction(instruction_t* instruction) {
    char* call = (char*) malloc(50 * sizeof(char));
    sprintf(call, "\tcall    %s", instruction->s1->name);
    return call;
}


char* create_enter_instruction(instruction_t* instruction) {
    char* enter = prologue(instruction->s1->name);
    return enter;
}

char* create_leave_instruction(instruction_t* instruction) {
    return epilogue(instruction->s1->name);
}

char* create_glob_decl_instruction(instruction_t* instruction) {
    char* glob_decl = (char*) malloc(50 * sizeof(char));
    sprintf(glob_decl, "\t.comm    %s,4,4", instruction->s1->name);
    return glob_decl;
}

char* create_asm_instruction(instruction_t* instruction) {
    switch (instruction->type) {
        case ADD:
            return create_add_instruction(instruction);
        case SUB:
            return create_sub_instruction(instruction);
        case MUL:
            return create_mul_instruction(instruction);
        case DIV:
            return create_div_instruction(instruction);
        case MOD:
            return create_mod_instruction(instruction);
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
        case JMP:
            return create_jmp_instruction(instruction);
        case JE:
            return create_je_instruction(instruction);
        case JNE:
            return create_jne_instruction(instruction);
        case LBL:
            return create_lbl_instruction(instruction);
        case CALL:
            return create_call_instruction(instruction);
        case ENTER:
            return create_enter_instruction(instruction);
        case LEAVE:
            return create_leave_instruction(instruction);
        case GLB:
            return create_glob_decl_instruction(instruction);
        default:
            printf("Invalid instruction type\n");
            exit(1);
    }
}

void create_asm(char* filename, list_t* instruction_seq) {
    FILE* f = fopen(filename, "w+");
    if (f == NULL) {
        printf("Error while attempting to create the asm file\n");
        exit(1);
    }
    
    node_t* cursor = instruction_seq->head->next;
    while (cursor != NULL) {
        instruction_t* instruction = (instruction_t*) cursor->value;
        char* asm_instruction = create_asm_instruction(instruction);
        fprintf(f, "%s\n", asm_instruction);
        cursor = cursor->next;
    }

    fclose(f);
}
