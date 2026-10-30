#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "cpu.h"
#include "asm.h"
#include "helpers.h"

/* Maps each supported assembly mnemonic to its CPU opcode and operand metadata. */
static const Instruction instruction_table[]= {
    {"LDA", INS_LD, true},
    {"HLT", INS_HLT, false},
    {"STA", INS_ST, true},
    {"ADD", INS_ADD, false},
    {"JMP", INS_JMP, true},
    {"JZ", INS_JZ,true},
    {"NOP", INS_NOP,false},
    {"SUB", INS_SUB,false},
    {"AND", INS_AND,false},
    {"OR", INS_OR,false},
    {"XOR", INS_XOR,false},
    {"DEC", INS_DEC,false},
    {"INC", INS_INC,false},
    {"OUT", INS_OUT,false}
};
/* Number of entries in instruction_table, without hard-coding its length. */
#define TABLE_SIZE ( sizeof(instruction_table) / sizeof(instruction_table[0]))

int main (int argc, char* argv[]){

    /* Use output.bin unless the caller supplies an output name. */
    char *output = "output.bin";
    static char custom_name[256];

    /* Require an input file and allow at most one optional output name. */
    if(argc >3 || argc <2){
        fprintf(stderr,"Invalid number of arguments\n");
        fprintf(stderr,"usage: %s <input,txt> [] output name",argv[0]);
        return 1;
    }

    /* Open the assembly source as a text file. */
    char *input = argv[1];
    FILE *source = fopen(input,"r");
    if(source == NULL){
        fprintf(stderr,"Error: Could not open the file %s",input);
        return 1;
    }

    if(argc == 3){
        /* The optional name is a base name; the assembler adds the .bin suffix. */
        snprintf(custom_name,sizeof(custom_name),"%s.bin",argv[2]);
        output = custom_name;
    }

    /* Create/truncate the output file in binary mode. */
    FILE *binary_file = fopen(output,"wb");
    if(binary_file == NULL){
        fprintf(stderr,"Could not create binary");
    }

    /* Assembly parsing and instruction encoding have not been implemented here yet. */

    /* Buffer for one source line; line_number tracks its position for future diagnostics. */
    char line[256];
    uint32_t line_number=0;
    
    #if DEBUG
        /* Show which source and output paths this run is using. */
        printf("Assembling %s -> %s...\n", input, output);
    #endif

    /* Read the source one line at a time. The loop body is currently a placeholder. */
    while (fgets(line,sizeof(line),source) != NULL) {
        line_number++;
    }

    /* Release the files opened by this function. */
    fclose(source);
    fclose(binary_file);
    return 0;
}
/* Tokenize a line: return 1 for a valid instruction, 0 for blank/comment-only,
 * or -1 for an invalid mnemonic or tokens that do not fit the parsed fields.
 */
int tokenize_line(char *raw_line, ParsedLine *parsed_data){
    /* Reject missing inputs before inspecting or writing through either pointer. */
    if (raw_line == NULL || parsed_data == NULL) {
        return -1;
    }

    /* Clear all fields so a failed parse cannot leave values from an earlier line. */
    parsed_data->opcode[0] = '\0';
    parsed_data->arg1[0] = '\0';
    parsed_data->arg2[0] = '\0';

    /* Treat everything from '#' to the end of the line as a comment. */
    char *comment = strchr(raw_line,'#');
    if (comment != NULL){ *comment = '\0';}

    /* Split in place on whitespace and commas; strtok modifies raw_line. */
    char *token = strtok(raw_line," ,\t\r\n");
    /* No token means the line was blank or contained only a comment. */
    if(token == NULL){return 0;}

    /* Store the mnemonic without truncation, then reject names absent from the ISA table. */
    if (c_stringcpy(parsed_data->opcode, token, sizeof(parsed_data->opcode)) != 0 ||
        is_mnemonic(parsed_data->opcode, instruction_table, TABLE_SIZE) != 1) {
        return -1;
    }

    /* Store up to two operands; c_stringcpy reports tokens too large for their fields. */
    token = strtok(NULL," ,\t\r\n");
    if(token != NULL){
        if (c_stringcpy(parsed_data->arg1, token, sizeof(parsed_data->arg1)) != 0) {
            return -1;
        }
    }

    /* arg2 is typically a source register, routing byte, or immediate payload. */
    token = strtok(NULL," ,\t\r\n");
    if(token != NULL){
        if (c_stringcpy(parsed_data->arg2, token, sizeof(parsed_data->arg2)) != 0) {
            return -1;
        }
    }

    /* Reject extra operands instead of silently ignoring input the parser cannot store. */
    if (strtok(NULL, " ,\t\r\n") != NULL) {
        return -1;
    }

    /* The caller can now distinguish this instruction from blank lines and parse errors. */
    return 1;
}
