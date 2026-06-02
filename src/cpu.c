#include "cpu.h"
#include "alu.h"
#include <stdio.h>

void cpu_unit(cpu* current_cpu) {
    uint8_t current_instruction = current_cpu->ram[current_cpu->program_counter];
    current_cpu->program_counter++;
        
    switch (current_instruction) {
        case INS_NOP: break;
        
        /* Memory Operations */
        case INS_LD:  execute_load(current_cpu); break;
        case INS_ST:  execute_store(current_cpu); break;
        
        /* Arithmetic */
        case INS_ADD: execute_arithmetic(current_cpu, ALU_ADD); break;
        case INS_SUB: execute_arithmetic(current_cpu, ALU_SUB); break;
        case INS_AND: execute_arithmetic(current_cpu, ALU_AND); break;
        case INS_OR:  execute_arithmetic(current_cpu, ALU_OR); break;
        case INS_XOR: execute_arithmetic(current_cpu, ALU_XOR); break;
        
        /* Unary */
        case INS_INC: execute_unary(current_cpu, ALU_INC); break;
        case INS_DEC: execute_unary(current_cpu, ALU_DEC); break;
        
        /* Control Flow */
        case INS_JMP: execute_jmp(current_cpu); break;
        case INS_JZ:  execute_jz(current_cpu); break;
        
        case INS_OUT: 
            printf("Value: %02X\n", current_cpu->registers[(current_cpu->ram[current_cpu->program_counter] & DESTINATION_MASK) >> 2]);
            current_cpu->program_counter++;
            break;
        /* Stop the machine*/    
        case INS_HLT: 
            current_cpu->is_halted = true; 
            break;
            
        default:
            printf("Hardware Fault: Unknown Opcode\n");
            current_cpu->is_halted = true;
            break;
    }    
}

void execute_arithmetic(cpu* current_cpu, uint8_t alu_operation) {
    uint8_t routing = current_cpu->ram[current_cpu->program_counter];
    uint8_t destination = (routing & DESTINATION_MASK) >> 2;
    uint8_t source = routing & SOURCE_MASK;
    
    /* Temp store value in A for overflow/underflow checks */
    uint8_t original_dest_value = current_cpu->registers[destination];
    uint8_t operand_value;

    /* Routing */
    if (ISIMMEDIATE(routing) && !ISMEMORY(routing)) {
        current_cpu->program_counter++;
        operand_value = current_cpu->ram[current_cpu->program_counter];
    } 
    else if (!ISIMMEDIATE(routing) && ISMEMORY(routing)) {
        current_cpu->program_counter++;
        uint8_t address = current_cpu->ram[current_cpu->program_counter];
        operand_value = current_cpu->ram[address];
    } 
    else if (ISIMMEDIATE(routing) && ISMEMORY(routing)) {
        operand_value = current_cpu->ram[current_cpu->registers[source]];
    } 
    else {
        operand_value = current_cpu->registers[source];
    }

    /* Execute ALU */
    current_cpu->registers[destination] = alu_compute(alu_operation, original_dest_value, operand_value);
    alu_update_flags(current_cpu, current_cpu->registers[destination]);
    
    /* Clear Carry flag by default */
    current_cpu->status_flags &= ~FLAG_C;

    /* specific flag handling */
    if (alu_operation == ALU_ADD) {
        if (current_cpu->registers[destination] < original_dest_value) {
            current_cpu->status_flags |= FLAG_C; // Overflow
        }
    } else if (alu_operation == ALU_SUB) {
        if (original_dest_value < operand_value) {
            current_cpu->status_flags |= FLAG_C; // Underflow
        }
    }

    /* Final PC increment to step over the instruction */
    current_cpu->program_counter++;
}

void execute_load(cpu* current_cpu) {
    uint8_t routing = current_cpu->ram[current_cpu->program_counter];
    uint8_t destination = (routing & DESTINATION_MASK) >> 2;
    uint8_t source = routing & SOURCE_MASK;

    /* Register Indirect */
    if (ISIMMEDIATE(routing) && ISMEMORY(routing)) {
        current_cpu->registers[destination] = current_cpu->ram[current_cpu->registers[source]];
    }
    /* Immediate Only */
    else if (ISIMMEDIATE(routing) && !ISMEMORY(routing)) {
        current_cpu->program_counter++;
        current_cpu->registers[destination] = current_cpu->ram[current_cpu->program_counter];
    }
    /* Memory Direct */
    else if (!ISIMMEDIATE(routing) && ISMEMORY(routing)) {
        current_cpu->program_counter++;
        uint8_t address = current_cpu->ram[current_cpu->program_counter];
        current_cpu->registers[destination] = current_cpu->ram[address];
    }
    /* Direct R to R */
    else {
        current_cpu->registers[destination] = current_cpu->registers[source];
    }
    current_cpu->program_counter++;
}

void execute_store(cpu* current_cpu) {
    uint8_t routing = current_cpu->ram[current_cpu->program_counter];
    uint8_t destination = (routing & DESTINATION_MASK) >> 2;
    uint8_t source = routing & SOURCE_MASK;

    /* Cannot store to a raw immediate number */
    if (ISIMMEDIATE(routing) && !ISMEMORY(routing)) {
        fprintf(stderr, "Hardware Fault: Illegal memory access in ST\n");
        current_cpu->is_halted = true;
        return; /* Abort execution immediately */
    }

    /* Register Indirect */
    if (ISIMMEDIATE(routing) && ISMEMORY(routing)) {
        uint8_t address = current_cpu->registers[destination];
        current_cpu->ram[address] = current_cpu->registers[source];
    }
    /* Memory Direct */
    else if (!ISIMMEDIATE(routing) && ISMEMORY(routing)) {
        current_cpu->program_counter++;
        uint8_t address = current_cpu->ram[current_cpu->program_counter];
        current_cpu->ram[address] = current_cpu->registers[source];
    }
    /* Direct R to R */
    else {
        current_cpu->registers[destination] = current_cpu->registers[source];
    }
    current_cpu->program_counter++;
}

void execute_unary(cpu* current_cpu, uint8_t alu_operation) {
    uint8_t routing = current_cpu->ram[current_cpu->program_counter];
    uint8_t destination = (routing & DESTINATION_MASK) >> 2;

    /* strictly target registers */
    if (ISIMMEDIATE(routing) || ISMEMORY(routing)) {
        fprintf(stderr, "Hardware Fault: Illegal memory access in Unary instruction\n");
        current_cpu->is_halted = true;
        return; /* Abort execution */
    }

    /* Execute ALU with 0 as the second operand */
    current_cpu->registers[destination] = alu_compute(alu_operation, current_cpu->registers[destination], 0);
    alu_update_flags(current_cpu, current_cpu->registers[destination]);
    
    current_cpu->program_counter++;
}

void execute_jmp(cpu* current_cpu) {
    uint8_t routing = current_cpu->ram[current_cpu->program_counter];
    uint8_t source = routing & SOURCE_MASK;

    /* Hardware Panic */
    if (!ISIMMEDIATE(routing)) {
        fprintf(stderr, "Hardware Fault: Illegal memory access in JMP\n");
        current_cpu->is_halted = true;
        return;
    }

    /* Immediate Mode */
    if (!ISMEMORY(routing)) {
        current_cpu->program_counter++;
        uint8_t address = current_cpu->ram[current_cpu->program_counter];
        current_cpu->program_counter = address;
    }
    /* Register Indirect */
    else {
        current_cpu->program_counter = current_cpu->registers[source];
    }
}

void execute_jz(cpu* current_cpu) {
    uint8_t routing = current_cpu->ram[current_cpu->program_counter];
    uint8_t source = routing & SOURCE_MASK;

    /* Hardware Panic */
    if (!ISIMMEDIATE(routing)) {
        fprintf(stderr, "Hardware Fault: Illegal memory access in JZ\n");
        current_cpu->is_halted = true;
        return;
    }

    /* Execute Jump if Zero Flag is ON */
    if ((current_cpu->status_flags & FLAG_ZERO) != 0) {
        if (!ISMEMORY(routing)) {
            current_cpu->program_counter++;
            uint8_t address = current_cpu->ram[current_cpu->program_counter];
            current_cpu->program_counter = address;
        } else {
            current_cpu->program_counter = current_cpu->registers[source];
        }
    } 
    /* Skip Jump if Zero Flag is OFF */
    else {
        if (!ISMEMORY(routing)) {
            current_cpu->program_counter = current_cpu->program_counter + 2;
        } else {
            current_cpu->program_counter++;
        }
    }
}