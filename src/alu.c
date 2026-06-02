#include "alu.h"


uint8_t alu_compute(uint8_t operation,uint8_t operand_a, uint8_t operand_b){
    /* Compute operations, flags are handled in the cpu switch.*/
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
        return 0;
    }
}

void alu_update_flags(cpu *cpu,uint8_t result){
    /* clear flags*/
    cpu->status_flags &= ~(FLAG_ZERO | FLAG_NEG);
    /* Setting flags*/
    if (result == 0){ cpu->status_flags |= FLAG_ZERO;}
    if ((result & 0x80) != 0){cpu->status_flags |= FLAG_NEG;}

}