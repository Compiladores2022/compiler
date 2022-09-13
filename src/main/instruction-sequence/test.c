#include "instruction.h"
#include "../symbol/symbol.h"

int main() {
    symbol_t* s1 = create_symbol();
    set_name(s1, "s1");
    symbol_t* s2 = create_symbol();
    set_name(s2, "s2");
    symbol_t* s3 = create_symbol();
    set_name(s3, "s3");
    instruction_t* instruction = create_instruction(AND, s1, s2, s3);
    return 0;
}
