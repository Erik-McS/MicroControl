#include "alu.h"

/* Apply an ALU operation to the supplied 8-bit operands and return its result.
 * The caller is responsible for updating CPU status flags.
 */
uint8_t alu_compute(uint8_t operation,uint8_t operand_a, uint8_t operand_b){
    /* uint8_t arithmetic naturally wraps modulo 256. */
    switch (operation)
    {
    case ALU_ADD:
        return operand_a + operand_b;
    case ALU_SUB:
        return operand_a - operand_b;
    case ALU_AND:
        return operand_a & operand_b;
    case ALU_OR:
        return operand_a | operand_b;
    case ALU_XOR:
        return operand_a ^ operand_b;
    case ALU_INC:
        return operand_a +1;
    case ALU_DEC:
        return operand_a -1;  
    default:
        /* Unknown operation codes produce zero; no status flags are changed here. */
        return 0;
    }
}

/* Update the result-dependent flags, preserving carry and overflow. */
void alu_update_flags(cpu *cpu,uint8_t result){
    /* Zero and negative describe the latest result, so clear their old values. */
    cpu->status_flags &= ~(FLAG_ZERO | FLAG_NEG);

    /* A zero result sets ZERO; bit 7 set means the 8-bit result is negative. */
    if (result == 0){ cpu->status_flags |= FLAG_ZERO;}
    if ((result & 0x80) != 0){cpu->status_flags |= FLAG_NEG;}

}