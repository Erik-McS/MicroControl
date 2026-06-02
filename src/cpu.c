#include "cpu.h"
#include "alu.h"
#include <stdio.h>

void cpu_unit(cpu* current_cpu){
        /* fetch */
        uint8_t current_instruction = current_cpu->ram[current_cpu->program_counter];
        current_cpu->program_counter ++;
        
        /* decode */
        switch(current_instruction){
            /* 2 bytes instruction*/
            case INS_LDA:
                /* move to the value to load in reg_a and load it*/
                current_cpu->reg_a = current_cpu->ram[current_cpu->program_counter];
                current_cpu->program_counter++;
                break;
            case INS_LDB:
                /* move to the value to load in reg_a and load it*/
                current_cpu->reg_b = current_cpu->ram[current_cpu->program_counter];
                current_cpu->program_counter++;
                break;
            case INS_NOP:
                /* do nothing*/
                break;
            /* Addition */    
            case INS_ADD:{
                /* temp store value in A for overflow check*/
                uint8_t real_a = current_cpu->reg_a;                
                current_cpu->reg_a = alu_compute(ALU_ADD,real_a,current_cpu->reg_b);
                alu_update_flags(current_cpu,current_cpu->reg_a);
                /* set carry flag to zero*/
                current_cpu->status_flags &= ~FLAG_C;
                /* check if overflow: reg_a < real_a */
                if(current_cpu->reg_a < real_a){current_cpu->status_flags |= FLAG_C;}
                break;                
            }
            case INS_SUB:
                current_cpu->reg_a = alu_compute(ALU_SUB,current_cpu->reg_a,current_cpu->reg_b);
                alu_update_flags(current_cpu,current_cpu->reg_a);
                break;                
            case INS_AND:
                current_cpu->reg_a = alu_compute(ALU_AND,current_cpu->reg_a,current_cpu->reg_b);
                alu_update_flags(current_cpu,current_cpu->reg_a);
                /* clear the carry flag*/
                current_cpu->status_flags &= ~FLAG_C;
                break;
            case INS_OR:
                current_cpu->reg_a = alu_compute(ALU_OR,current_cpu->reg_a,current_cpu->reg_b);
                alu_update_flags(current_cpu,current_cpu->reg_a);
                current_cpu->status_flags &= ~FLAG_C;
                break;
            case INS_XOR:
                current_cpu->reg_a = alu_compute(ALU_XOR,current_cpu->reg_a,current_cpu->reg_b);
                alu_update_flags(current_cpu,current_cpu->reg_a);
                current_cpu->status_flags &= ~FLAG_C;
                break;
            case INS_INC:
                current_cpu->reg_a = alu_compute(ALU_INC,current_cpu->reg_a,current_cpu->reg_b);
                alu_update_flags(current_cpu,current_cpu->reg_a);
                break;
            case INS_DEC:
                current_cpu->reg_a = alu_compute(ALU_DEC,current_cpu->reg_a,current_cpu->reg_b);
                alu_update_flags(current_cpu,current_cpu->reg_a);
                break;
            case INS_HLT:
                /* exit*/
                current_cpu->is_halted = true;
                break;
            case INS_JMP:
                /* update PC with the jump address*/
                current_cpu->program_counter = current_cpu->ram[current_cpu->program_counter];
                break;
            case INS_OUT:
                printf("Value in reg_a: %02X\n",current_cpu->reg_a);
                break;
                /* Store reg_a value in ram at a given address*/
            case INS_STA:{
                uint8_t address = current_cpu->ram[current_cpu->program_counter];
                current_cpu->ram[address] = current_cpu->reg_a;
                current_cpu->program_counter++;
                break;}
            case INS_STB:{
                uint8_t address = current_cpu->ram[current_cpu->program_counter];
                current_cpu->ram[address] = current_cpu->reg_b;
                current_cpu->program_counter++;
                break;}                
            /* jump if zero*/
            case INS_JZ:{
                uint8_t address = current_cpu->ram[current_cpu->program_counter];
                /* bitwise AND to check the zero flag bit */
                if((current_cpu->status_flags & FLAG_ZERO) !=0){
                    current_cpu->program_counter = address;}
                else {current_cpu->program_counter++;}
                break;}
            default:
                printf("Unknown command, exiting\n");
                current_cpu->is_halted = true;
                break;
        }    
}