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

typedef struct 
{
    char opcode[16];
    char arg1[32];
    char arg2[32];
}ParsedLine;



/* Returns 1 if a valid instruction was parsed, 0 if it was an empty line/comment */
int tokenize_line(char *raw_line, ParsedLine *parsed_data);

/* Returns 0 on success, -1 on failure */
int generate_binary(ParsedLine *parsed_data, FILE *output_file, uint32_t line_number);

#endif