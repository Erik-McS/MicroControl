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
    micro_cpu.is_halted = false;
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
        printf("memory at index 0: %02X \n",micro_cpu.ram[0]);
        printf("memory at index 1: %02X \n",micro_cpu.ram[1]);
        printf("memory at index 2: %02X \n",micro_cpu.ram[2]);
    #endif
    printf("Value in reg_a: %02X \n", micro_cpu.reg_a);
    return 0;
}

