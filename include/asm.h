#ifndef ASM_H
#define ASM_H

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

/* struct to hold the instructions */

typedef struct
{
    /* data */
    const char *instruction;
    uint8_t opcode;
    bool need_operand;
}Instruction;

/* fct to output the 'machine code' */
int output_code(FILE *output, const char *instruction, const char *argument,uint32_t line_number);

#endif