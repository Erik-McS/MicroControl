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
            case INS_LD:{
                /* getting the needed data */
                uint8_t routing = current_cpu->ram[current_cpu->program_counter];
                uint8_t destination = (routing & DESTINATION_MASK) >> 2;
                uint8_t source = routing & SOURCE_MASK;

                /* register indirect*/
                if ( (ISIMMEDIATE(routing)) && (ISMEMORY(routing)) ){
                    current_cpu->registers[destination] = current_cpu->ram[current_cpu->registers[source]];
                    current_cpu->program_counter++;
                    break;
                }
                /* Immediate only */
                if ( (ISIMMEDIATE(routing)) && !(ISMEMORY(routing)) ){
                    current_cpu->program_counter++;
                    current_cpu->registers[destination] = current_cpu->ram[current_cpu->program_counter];
                    current_cpu->program_counter++;
                    break;
                }
                /* Memory only */
                if ( !(ISIMMEDIATE(routing)) && (ISMEMORY(routing)) ){
                    current_cpu->program_counter++;
                    uint8_t address = current_cpu->ram[current_cpu->program_counter];
                    current_cpu->registers[destination] = current_cpu->ram[address];
                    current_cpu->program_counter++;
                    break;
                }
                /* direct R to R*/
                current_cpu->registers[destination] = current_cpu->registers[source];
                current_cpu->program_counter++;
                break;}
            case INS_NOP:
                /* do nothing*/
                break;
            /* Addition */    
            case INS_ADD:{
                uint8_t routing = current_cpu->ram[current_cpu->program_counter];
                uint8_t destination = (routing & DESTINATION_MASK) >> 2;
                uint8_t source = routing & SOURCE_MASK;
                /* temp store value in A for overflow check*/
                uint8_t real_a = current_cpu->registers[destination];
                uint8_t operand_value;
                /* Immediate */
                if((ISIMMEDIATE(routing))&& !(ISMEMORY(routing))){
                    current_cpu->program_counter++;
                    operand_value = current_cpu->ram[current_cpu->program_counter];
                }
                /* memory direct*/
                else if(!(ISIMMEDIATE(routing))&&(ISMEMORY(routing))){
                    current_cpu->program_counter++;
                    uint8_t address = current_cpu->ram[current_cpu->program_counter];
                    operand_value = current_cpu->ram[address];
                }
                /* indirect register*/
                else if((ISIMMEDIATE(routing))&&(ISMEMORY(routing))){
                    operand_value = current_cpu->ram[current_cpu->registers[source]];
                }
                else{
                    /* R to R */
                    operand_value = current_cpu->registers[source];
                }
                /* ADD dest + source */
                current_cpu->registers[destination] = alu_compute(ALU_ADD,real_a,operand_value);
                alu_update_flags(current_cpu,current_cpu->registers[destination]);
                /* set carry flag to zero*/
                current_cpu->status_flags &= ~FLAG_C;
                /* check if overflow: reg_a < real_a */
                if(current_cpu->registers[destination] < real_a){current_cpu->status_flags |= FLAG_C;}
                current_cpu->program_counter++;
                break;                
            }
            case INS_SUB:{
                uint8_t routing = current_cpu->ram[current_cpu->program_counter];
                uint8_t destination = (routing & DESTINATION_MASK) >> 2;
                uint8_t source = routing & SOURCE_MASK;
                /* temp store value in A for overflow check*/
                uint8_t real_a = current_cpu->registers[destination];
                uint8_t operand_value;
                /* Immediate */
                if((ISIMMEDIATE(routing))&& !(ISMEMORY(routing))){
                    current_cpu->program_counter++;
                    operand_value = current_cpu->ram[current_cpu->program_counter];
                }
                /* memory direct*/
                else if(!(ISIMMEDIATE(routing))&&(ISMEMORY(routing))){
                    current_cpu->program_counter++;
                    uint8_t address = current_cpu->ram[current_cpu->program_counter];
                    operand_value = current_cpu->ram[address];
                }
                /* indirect register*/
                else if((ISIMMEDIATE(routing))&&(ISMEMORY(routing))){
                    operand_value = current_cpu->ram[current_cpu->registers[source]];
                }
                else{
                    /* R to R */
                    operand_value = current_cpu->registers[source];
                }
                current_cpu->registers[destination] = alu_compute(ALU_SUB,real_a,operand_value);
                alu_update_flags(current_cpu,current_cpu->registers[destination]);
                /* set carry flag to zero */
                current_cpu->status_flags &= ~FLAG_C;
                /* check for underflow */
                if (real_a < operand_value) {
                    current_cpu->status_flags |= FLAG_C;
                }
                current_cpu->program_counter++;
                break;}                
            case INS_AND:{
                uint8_t routing = current_cpu->ram[current_cpu->program_counter];
                uint8_t destination = (routing & DESTINATION_MASK) >> 2;
                uint8_t source = routing & SOURCE_MASK;
                /* temp store value in A for overflow check*/
                uint8_t real_a = current_cpu->registers[destination];
                uint8_t operand_value;
                /* Immediate */
                if((ISIMMEDIATE(routing))&& !(ISMEMORY(routing))){
                    current_cpu->program_counter++;
                    operand_value = current_cpu->ram[current_cpu->program_counter];
                }
                /* memory direct*/
                else if(!(ISIMMEDIATE(routing))&&(ISMEMORY(routing))){
                    current_cpu->program_counter++;
                    uint8_t address = current_cpu->ram[current_cpu->program_counter];
                    operand_value = current_cpu->ram[address];
                }
                /* indirect register*/
                else if((ISIMMEDIATE(routing))&&(ISMEMORY(routing))){
                    operand_value = current_cpu->ram[current_cpu->registers[source]];
                }
                else{
                    /* R to R */
                    operand_value = current_cpu->registers[source];
                }
                current_cpu->registers[destination] = alu_compute(ALU_AND,real_a,operand_value);
                alu_update_flags(current_cpu,current_cpu->registers[destination]);
                /* clear the carry flag*/
                current_cpu->status_flags &= ~FLAG_C;
                current_cpu->program_counter++;
                break;}
            case INS_OR:{
                uint8_t routing = current_cpu->ram[current_cpu->program_counter];
                uint8_t destination = (routing & DESTINATION_MASK) >> 2;
                uint8_t source = routing & SOURCE_MASK;
                /* temp store value in A for overflow check*/
                uint8_t real_a = current_cpu->registers[destination];
                uint8_t operand_value;
                /* Immediate */
                if((ISIMMEDIATE(routing))&& !(ISMEMORY(routing))){
                    current_cpu->program_counter++;
                    operand_value = current_cpu->ram[current_cpu->program_counter];
                }
                /* memory direct*/
                else if(!(ISIMMEDIATE(routing))&&(ISMEMORY(routing))){
                    current_cpu->program_counter++;
                    uint8_t address = current_cpu->ram[current_cpu->program_counter];
                    operand_value = current_cpu->ram[address];
                }
                /* indirect register*/
                else if((ISIMMEDIATE(routing))&&(ISMEMORY(routing))){
                    operand_value = current_cpu->ram[current_cpu->registers[source]];
                }
                else{
                    /* R to R */
                    operand_value = current_cpu->registers[source];
                }
                current_cpu->registers[destination] = alu_compute(ALU_OR,real_a,operand_value);
                alu_update_flags(current_cpu,current_cpu->registers[destination]);
                /* clear the carry flag*/
                current_cpu->status_flags &= ~FLAG_C;
                current_cpu->program_counter++;
                break;}
            case INS_XOR:{
                uint8_t routing = current_cpu->ram[current_cpu->program_counter];
                uint8_t destination = (routing & DESTINATION_MASK) >> 2;
                uint8_t source = routing & SOURCE_MASK;
                /* temp store value in A for overflow check*/
                uint8_t real_a = current_cpu->registers[destination];
                uint8_t operand_value;
                /* Immediate */
                if((ISIMMEDIATE(routing))&& !(ISMEMORY(routing))){
                    current_cpu->program_counter++;
                    operand_value = current_cpu->ram[current_cpu->program_counter];
                }
                /* memory direct*/
                else if(!(ISIMMEDIATE(routing))&&(ISMEMORY(routing))){
                    current_cpu->program_counter++;
                    uint8_t address = current_cpu->ram[current_cpu->program_counter];
                    operand_value = current_cpu->ram[address];
                }
                /* indirect register*/
                else if((ISIMMEDIATE(routing))&&(ISMEMORY(routing))){
                    operand_value = current_cpu->ram[current_cpu->registers[source]];
                }
                else{
                    /* R to R */
                    operand_value = current_cpu->registers[source];
                }
                current_cpu->registers[destination] = alu_compute(ALU_XOR,real_a,operand_value);
                alu_update_flags(current_cpu,current_cpu->registers[destination]);
                /* clear the carry flag*/
                current_cpu->status_flags &= ~FLAG_C;
                current_cpu->program_counter++;
                break;}
            case INS_INC:{
                uint8_t routing = current_cpu->ram[current_cpu->program_counter];
                uint8_t destination = (routing & DESTINATION_MASK) >> 2;
                /* Invalid ST*/    
                if( (ISIMMEDIATE(routing)) && (ISMEMORY(routing))){
                    fprintf(stderr,"Invalid operation, illegal memory access");
                    current_cpu->is_halted = true;
                    break;
                }
                current_cpu->registers[destination] = alu_compute(ALU_INC,current_cpu->registers[destination],0);
                alu_update_flags(current_cpu,current_cpu->registers[destination]);
                current_cpu->program_counter++;
                break;}
            case INS_DEC:{
                uint8_t routing = current_cpu->ram[current_cpu->program_counter];
                uint8_t destination = (routing & DESTINATION_MASK) >> 2;
                /* Invalid ST*/    
                if( (ISIMMEDIATE(routing)) && (ISMEMORY(routing))){
                    fprintf(stderr,"Invalid operation, illegal memory access");
                    current_cpu->is_halted = true;
                    break;
                }
                current_cpu->registers[destination] = alu_compute(ALU_DEC,current_cpu->registers[destination],0);
                alu_update_flags(current_cpu,current_cpu->registers[destination]);
                current_cpu->program_counter++;
                break;}
            case INS_HLT:
                /* exit*/
                current_cpu->is_halted = true;
                break;
            case INS_JMP:{
                uint8_t routing = current_cpu->ram[current_cpu->program_counter];
                uint8_t source = routing & SOURCE_MASK;
                if( !(ISIMMEDIATE(routing)) ){
                    fprintf(stderr,"Invalid operation, illegal memory access");
                    current_cpu->is_halted = true;
                    break;
                }
                /* immediate mode*/
                if( (ISIMMEDIATE(routing)) && !(ISMEMORY(routing)) ){
                    current_cpu->program_counter++;
                    uint8_t address = current_cpu->ram[current_cpu->program_counter];
                    current_cpu->program_counter = address;
                    break;
                }
                /* from register*/
                else if( (ISIMMEDIATE(routing)) && (ISMEMORY(routing))){
                    current_cpu->program_counter = current_cpu->registers[source];
                    break;
                }
            }
            case INS_OUT:{
                uint8_t routing = current_cpu->ram[current_cpu->program_counter];
                uint8_t destination = (routing & DESTINATION_MASK) >> 2;
                printf("Value in reg_a: %02X\n",current_cpu->registers[destination]);
                current_cpu->program_counter++;
                break;}
            /* Store reg_a value in ram at a given address*/
            case INS_ST:{
                /* getting the needed data */
                uint8_t routing = current_cpu->ram[current_cpu->program_counter];
                uint8_t destination = (routing & DESTINATION_MASK) >> 2;
                uint8_t source = routing & SOURCE_MASK;
                /* Invalid ST*/    
                if( (ISIMMEDIATE(routing)) && !(ISMEMORY(routing))){
                    fprintf(stderr,"Invalid operation, illegal memory access");
                    current_cpu->is_halted = true;
                    break;
                }
                /* register indirect*/
                if( (ISIMMEDIATE(routing)) && (ISMEMORY(routing))){
                    uint8_t address = current_cpu->registers[destination];
                    current_cpu->ram[address] = current_cpu->registers[source];
                    current_cpu->program_counter++;
                    break;
                }
                /* Memory direct*/
                if( !(ISIMMEDIATE(routing)) && (ISMEMORY(routing))){
                    current_cpu->program_counter++;
                    uint8_t address = current_cpu->ram[current_cpu->program_counter];
                    current_cpu->ram[address] = current_cpu->registers[source];
                    current_cpu->program_counter++;
                    break;
                }    
                /* R to R */
                current_cpu->registers[destination] = current_cpu->registers[source];
                current_cpu->program_counter++;
                break;}              
            /* jump if zero*/
            case INS_JZ:{
                /* bitwise AND to check the zero flag bit */
                uint8_t routing = current_cpu->ram[current_cpu->program_counter];
                /* Checking illegal mode*/   
                if( !(ISIMMEDIATE(routing)) ){
                        fprintf(stderr,"Invalid operation, illegal memory access");
                        current_cpu->is_halted = true;
                        break;
                    }
                if((current_cpu->status_flags & FLAG_ZERO) !=0){
                    uint8_t source = routing & SOURCE_MASK;
                    /* immediate mode*/
                    if(!(ISMEMORY(routing)) ){
                        current_cpu->program_counter++;
                        uint8_t address = current_cpu->ram[current_cpu->program_counter];
                        current_cpu->program_counter = address;
                        break;
                    }
                    /* from register*/
                    else if((ISMEMORY(routing))){
                        current_cpu->program_counter = current_cpu->registers[source];
                        break;
                    }                
                }
                else {
                    if(!(ISMEMORY(routing)) ){
                        current_cpu->program_counter = current_cpu->program_counter +2;
                        break;
                    }
                    else if((ISMEMORY(routing)) ){
                        current_cpu->program_counter++;
                        break;
                    }
                }
                break;}
            default:
                printf("Unknown command, exiting\n");
                current_cpu->is_halted = true;
                break;
        }    
}