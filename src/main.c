#include <stdio.h>
#include <stdbool.h>
#include "cpu.h"
#include "alu.h"


void debug_output(cpu *micro_cpu);

int main(int argc, char* argv[]) {

    /* testing correct argc size*/
    if (argc != 2){
        printf("Wrong number of arguments\n");
        return 1;
    }

    cpu micro_cpu = {0};
    micro_cpu.is_halted = false;
    micro_cpu.stack_pointer = 0xFF;
    FILE* rom;
    rom = fopen(argv[1],"rb");
    
    if(rom == NULL){printf("Error while opening the file\n"); return 1;}

    fread(micro_cpu.ram,1,MAX_RAM_SIZE,rom);
    fclose(rom);

    /* cpu loop -- Control Unit*/
    while (!micro_cpu.is_halted){
      cpu_unit(&micro_cpu);
    }
    #if DEBUG
        debug_output(&micro_cpu);
    #endif
    printf("Value in reg_a: %02X \n", micro_cpu.reg_a);
    return 0;
}

void debug_output(cpu *micro_cpu){
    printf("\n=========================================\n");
    printf("     MICROCONTROL POST-EXECUTION DUMP    \n");
    printf("=========================================\n");
    
    /* 1. Print Internal CPU Registers */
    printf("REGISTERS:\n");
    printf("  Reg A:  %02X\n", micro_cpu->reg_a);
    printf("  Reg B:  %02X\n", micro_cpu->reg_b); // Now that you have STB!
    printf("  PC:     %02X\n", micro_cpu->program_counter);
    printf("  Flags:  %02X\n", micro_cpu->status_flags);
    printf("\n");

    /* 2. Print RAM in a clean, professional Hex Grid */
    printf("RAM DUMP (First 16 Bytes):\n");
    printf("Addr | Bytes\n");
    printf("-----+-----------------------------------\n");
    printf("0x00 | ");
    
    for (int i = 0; i < 16; i++) {
        printf("%02X ", micro_cpu->ram[i]);
        
        /* Insert a clean line break every 8 bytes to keep it scannable */
        if ((i + 1) % 8 == 0 && i < 15) {
            printf("\n0x%02X | ", i + 1);
        }
    }
    printf("\n=========================================\n\n");   
}

