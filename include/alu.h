#ifndef ALU_H
#define ALU_H

#include <stdint.h>
#include "cpu.h"

typedef enum AluOperations{
    ALU_ADD,
    ALU_SUB,
    ALU_AND,
    ALU_OR,
    ALU_XOR,
    ALU_DEC,
    ALU_INC
}AluOperations;

uint8_t alu_compute (uint8_t operation, uint8_t operand_a, uint8_t operand_b);
void alu_update_flags(cpu *cpu,uint8_t result);
#endif