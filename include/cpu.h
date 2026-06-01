#ifndef CPU_H
# define CPU_H

#include <stdint.h>

#define MAX_RAM_SIZE 256
#define DEBUG 1


typedef enum Instructions{
    INS_NOP = 0x00,
    INS_LDA = 0x01,
    INS_HLT = 0xFF,
    INS_LDB = 0x02,
    INS_ADD = 0x03,
    INS_JMP = 0x04,
    INS_OUT = 0x05
}Instructions;

/* Defines the cpu*/
typedef struct cpu
{
    /* Array for ram */
    uint8_t ram[MAX_RAM_SIZE];
    /* Registers*/
    uint8_t reg_a;
    uint8_t reg_b;
    /* PC */
    uint8_t program_counter;
}cpu;

#endif