#ifndef ALU_H
#define ALU_H

#include <stdint.h>
#include "cpu.h"

/* Operations understood by alu_compute. Arithmetic results are 8-bit values. */
typedef enum AluOperations{
    ALU_ADD, /* Add operand_b to operand_a. */
    ALU_SUB, /* Subtract operand_b from operand_a. */
    ALU_AND, /* Bitwise AND of both operands. */
    ALU_OR,  /* Bitwise OR of both operands. */
    ALU_XOR, /* Bitwise exclusive OR of both operands. */
    ALU_DEC, /* Decrement operand_a by one. */
    ALU_INC  /* Increment operand_a by one. */
}AluOperations;

/* Compute an operation's result; this does not update CPU status flags. */
uint8_t alu_compute (uint8_t operation, uint8_t operand_a, uint8_t operand_b);

/* Set ZERO and NEG from result while preserving other status flags. */
void alu_update_flags(cpu *cpu,uint8_t result);
#endif