#ifndef ASM_H
#define ASM_H

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

/* One assembler-supported mnemonic and its corresponding machine encoding. */
typedef struct
{
    const char *instruction; /* Mnemonic as written in assembly source. */
    uint8_t opcode;          /* Opcode byte emitted for this instruction. */
    bool need_operand;       /* Whether the instruction expects an operand. */
}Instruction;

/* Text fields extracted from one non-empty source line. */
typedef struct 
{
    char opcode[16]; /* Mnemonic token. */
    char arg1[32];   /* First operand token, if present. */
    char arg2[32];   /* Second operand token or payload, if present. */
}ParsedLine;


/* Split a source line into opcode and up to two arguments.
 * Returns 1 for a non-empty line, or 0 for a blank/comment-only line.
 */
int tokenize_line(char *raw_line, ParsedLine *parsed_data);

/* Encode a parsed line to output_file; line_number supports source diagnostics.
 * Returns 0 on success or -1 on failure.
 */
int generate_binary(ParsedLine *parsed_data, FILE *output_file, uint32_t line_number);

#endif