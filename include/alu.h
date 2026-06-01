#ifndef ALU_H
#define ALU_H

#include <stdint.h>

typedef enum AluOperations{
    ALU_ADD
}AluOperations;

uint8_t alu_compute (uint8_t operation, uint8_t operand_a, uint8_t operand_b);

#endif