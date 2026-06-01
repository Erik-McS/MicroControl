#include "cpu.h"
#include "alu.h"
#include <stdio.h>

void cpu_unit(cpu* current_cpu){
        uint8_t current_instruction = current_cpu->ram[current_cpu->program_counter];

        switch(current_instruction){
            /* 2 bytes instruction*/
            case INS_LDA:
                /* move to the value to load in reg_a and load it*/
                current_cpu->program_counter++;
                current_cpu->reg_a = current_cpu->ram[current_cpu->program_counter];
                current_cpu->program_counter++;
                break;
            case INS_LDB:
                /* move to the value to load in reg_a and load it*/
                current_cpu->program_counter++;
                current_cpu->reg_b = current_cpu->ram[current_cpu->program_counter];
                current_cpu->program_counter++;
                break;
            case INS_NOP:
                /* do nothing*/
                current_cpu->program_counter++;
                break;
            /* Addition */    
            case INS_ADD:
                current_cpu->reg_a = alu_compute(ALU_ADD,current_cpu->reg_a,current_cpu->reg_b) ;
                current_cpu->program_counter++;
                break;
            case INS_HLT:
                /* exit*/
                current_cpu->is_halted = true;
                break;
            case INS_JMP:
                current_cpu->program_counter++;
                /* update PC with the jump address*/
                current_cpu->program_counter = current_cpu->ram[current_cpu->program_counter];
                break;
            case INS_OUT:
                current_cpu->program_counter++;
                printf("Value in reg_a: %02X\n",current_cpu->reg_a);
                break;
                /* Store reg_a value in ram at a given address*/
            case INS_STA:
                current_cpu->program_counter++;
                uint8_t address = current_cpu->ram[current_cpu->program_counter];
                current_cpu->ram[address] = current_cpu->reg_a;
                current_cpu->program_counter++;
                break;
            default:
                printf("Unknown command, exiting\n");
                current_cpu->is_halted = true;
                break;
        }
    
}