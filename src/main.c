#include <stdio.h>
#include <stdbool.h>
#include "cpu.h"
#include "alu.h"

int main(int argc, char* argv[]) {

    /* testing correct argc size*/
    if (argc != 2){
        printf("Wrong number of arguments\n");
        return 1;
    }

    cpu micro_cpu = {0};
    bool is_running = true;
    FILE* rom;
    rom = fopen(argv[1],"rb");
    
    if(rom == NULL){printf("Error while opening the file\n"); return 1;}

    fread(micro_cpu.ram,1,MAX_RAM_SIZE,rom);
    fclose(rom);

    /* cpu loop -- Control Unit*/
    while (is_running){
        /*get the instruction*/
        uint8_t current_instruction = micro_cpu.ram[micro_cpu.program_counter];

        switch(current_instruction){
            /* 2 bytes instruction*/
            case INS_LDA:
                /* move to the value to load in reg_a and load it*/
                micro_cpu.program_counter++;
                micro_cpu.reg_a = micro_cpu.ram[micro_cpu.program_counter];
                micro_cpu.program_counter++;
                break;
            case INS_LDB:
                /* move to the value to load in reg_a and load it*/
                micro_cpu.program_counter++;
                micro_cpu.reg_b = micro_cpu.ram[micro_cpu.program_counter];
                micro_cpu.program_counter++;
                break;
            case INS_NOP:
                /* do nothing*/
                micro_cpu.program_counter++;
                break;
            /* Addition */    
            case INS_ADD:
                micro_cpu.reg_a = alu_compute(ALU_ADD,micro_cpu.reg_a,micro_cpu.reg_b) ;
                micro_cpu.program_counter++;
                break;
            case INS_HLT:
                /* exit*/
                is_running = false;
                break;
            case INS_JMP:
                micro_cpu.program_counter++;
                /* update PC with the jump address*/
                micro_cpu.program_counter = micro_cpu.ram[micro_cpu.program_counter];
                break;
            case INS_OUT:
                micro_cpu.program_counter++;
                printf("Value in reg_a: %02X\n",micro_cpu.reg_a);
                break;
            default:
                printf("Unknown command, exiting\n");
                is_running = false;
                break;
        }
    }
    #if DEBUG
        printf("memory at index 0: %02X \n",micro_cpu.ram[0]);
        printf("memory at index 1: %02X \n",micro_cpu.ram[1]);
        printf("memory at index 2: %02X \n",micro_cpu.ram[2]);
    #endif
    printf("Value in reg_a: %02X \n", micro_cpu.reg_a);
    return 0;
}

