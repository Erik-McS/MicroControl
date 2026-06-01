#include "alu.h"


uint8_t alu_compute(uint8_t operation,uint8_t operand_a, uint8_t operand_b){


    switch (operation)
    {
    case ALU_ADD:
        return operand_a + operand_b;
        break;
    default:
        return 0;
        break;
    }
}