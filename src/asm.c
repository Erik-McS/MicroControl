#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "cpu.h"
#include "asm.h"

/*Instructions table*/
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
/* number of instructions in the table*/
#define TABLE_SIZE ( sizeof(instruction_table) / sizeof(instruction_table[0]))

int main (int argc, char* argv[]){

    char *output = "output.bin";
    static char custom_name[256];

    if(argc >3 || argc <2){
        fprintf(stderr,"Invalid number of arguments\n");
        fprintf(stderr,"usage: %s <input,txt> [] output name",argv[0]);
        return 1;
    }

    char *input = argv[1];
    FILE *source = fopen(input,"r");
    if(source == NULL){
        fprintf(stderr,"Error: Could not open the file %s",input);
        return 1;
    }

    if(argc == 3){
    snprintf(custom_name,sizeof(custom_name),"%s.bin",argv[2]);
    output = custom_name;        
    }

    FILE *binary_file = fopen(output,"wb");
    if(binary_file == NULL){
        fprintf(stderr,"Could not create binary");
    }

    /* -- parsing logic will go here */

    char line[256];
    uint32_t line_number=0;

    /* read till eof - fgets(char * destination, int size, FILE * fptr);*/
    
    #if DEBUG
        printf("Assembling %s -> %s...\n", input, output);
    #endif

    while (fgets(line,sizeof(line),source) != NULL) {
        line_number++;
        
        

     
    }

    /* Closing resources*/
    fclose(source);
    fclose(binary_file);
    return 0;
}

int tokenize_line(char *raw_line, ParsedLine *parsed_data){
    /* Strip inline comment, get a pointer to any # in the line */
    char *comment = strchr(raw_line,'#');
    /* if there is one, we replace it with \0 to skip the inline comment */
    if (comment != NULL){ *comment = '\0';}

    char *token = strtok(raw_line," ,\t\r\n");
    /* if empty, skip */
    if(token == NULL){return 0;}
    /* copy op */
    strncpy(parsed_data->opcode,token,sizeof(token)-1);
    parsed_data->opcode[sizeof(parsed_data->opcode)-1] = '\0'; /* add end of string */

    /* get first arg: destination*/
    token = strtok(NULL," ,\t\r\n");
    if(token != NULL){
        strncpy(parsed_data->arg1,token,sizeof(token)-1);
        parsed_data->arg1[sizeof(parsed_data->arg1)-1] = '\0';
    }
    else{
        parsed_data->arg1[0] = '\0';
    }
    /* get second arg: source/payload*/
    token = strtok(NULL," ,\t\r\n");
    if(token != NULL){
        strncpy(parsed_data->arg2,token,sizeof(token)-1);
        parsed_data->arg2[sizeof(parsed_data->arg2)-1] = '\0';
    }
    else{
        parsed_data->arg2[0] = '\0';
    }
    return 1;
}

