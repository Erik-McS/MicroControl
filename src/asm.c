#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "cpu.h"
#include "asm.h"

/*Instructions table*/
static const Instruction instruction_table[]= {
    {"LDA", INS_LDA, true},
    {"LDB", INS_LDB, true},
    {"HLT", INS_HLT, false},
    {"STA", INS_STA, true},
    {"STB", INS_STB, true},
    {"ADD", INS_ADD, false},
    {"JMP", INS_JMP, true},
    {"JZ", INS_JZ,true},
    {"NOP", INS_NOP,false},
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
        /* replace the \n at the end of the line by \0*/
        line[strcspn(line,"\n")] = '\0';
        /* Strip inline comment, get a pointer to any # in the line */
        char *comment = strchr(line,'#');
        /* if there is one, we replace it with \0 to skip the inline comment */
        if (comment != NULL){ *comment = '\0';}

        if(line[0] == '\0' || line[0] == '#') {continue;}
        /* storing instruction and argument*/
        char instruction[64]={0};
        char argument[64]={0};

        int arg_found = sscanf(line,"%63s %63s",instruction,argument);
        #if DEBUG
        /* if empty line*/
        if(arg_found == 0){continue;}
        if(arg_found == 1){printf("[Line %02d] Opcode: %s (no Operand)\n",line_number,instruction);}
        if(arg_found == 2){printf("[Line %02d] Opcode: %s Operand: %s\n",line_number,instruction,argument);}
        #endif
        /* output the line in file*/
        output_code(binary_file,instruction,argument,line_number);       
    }

    /* Closing resources*/
    fclose(source);
    fclose(binary_file);
    return 0;
}

/* lookup for instrcution in the table and output opcode/operand in the file */
int output_code(FILE *output, const char *instruction, const char *argument,uint32_t line_number){

    /* loop the entire table*/

    for(size_t i = 0; i < TABLE_SIZE;i++){
        /* Check if instruction found*/
        if ( strcmp(instruction,instruction_table[i].instruction) == 0){
            /* output the instruction in the file*/
            fputc(instruction_table[i].opcode,output);
            /* if instructions has an argument, output it*/
            if(instruction_table[i].need_operand == true){
                uint32_t op_value;
                /* check operand exists*/
                if(sscanf(argument,"%i",&op_value) !=1){
                    fprintf(stderr,"[Error Line %u]:No operand was given",line_number);
                    return -1;
                }
                fputc((uint8_t)op_value,output);    
            }
            return 0;
        }
    }
    /* Instruction not found*/
    fprintf(stderr,"[Error Line %u]: Unknown instruction found",line_number);
    return -1;
}