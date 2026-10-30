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
/* Tokenize a single line of assembly code into its components. 
    returns 1 if a non-empty line was parsed, 0 if the line was empty or a comment, and -1 if the mnemonic was invalid. */
int tokenize_line(char *raw_line, ParsedLine *parsed_data){
    /* Treat everything from '#' to the end of the line as a comment. */
    char *comment = strchr(raw_line,'#');
    if (comment != NULL){ *comment = '\0';}

    /* Split in place on whitespace and commas; strtok modifies raw_line. */
    char *token = strtok(raw_line," ,\t\r\n");
    /* No token means the line was blank or contained only a comment. */
    if(token == NULL){return 0;}

    /* The first token is the mnemonic/opcode text. */
    c_stringcpy(parsed_data->opcode,token,sizeof(parsed_data->opcode));
    if(is_mnemonic(parsed_data->opcode,instruction_table,TABLE_SIZE) !=1){return -1;} /* Invalid mnemonic */

    /* The next token is arg1 (typically a destination register). */
    token = strtok(NULL," ,\t\r\n");
    if(token != NULL){
        c_stringcpy(parsed_data->arg1,token,sizeof(parsed_data->arg1));
    }
    else{
        parsed_data->arg1[0] = '\0';
    }

    /* The following token is arg2 (typically a source or immediate payload). */
    token = strtok(NULL," ,\t\r\n");
    if(token != NULL){
        c_stringcpy(parsed_data->arg2,token,sizeof(parsed_data->arg2));
    }
    else{
        parsed_data->arg2[0] = '\0';
    }
    /* Additional tokens are not stored; report that a non-empty line was parsed. */
    return 1;
}
