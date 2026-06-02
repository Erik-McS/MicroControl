#ifndef CPU_H
# define CPU_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_RAM_SIZE 256
#define DEBUG 1

/* Status flags */
#define FLAG_ZERO (1U<<0)   /* 0000 0001 - equals 0*/
#define FLAG_NEG (1U<<1)    /* 0000 0010 - is negative */
#define FLAG_C (1U<<2)      /* 0000 0100 - has carry */
#define FLAG_O (1U<<3)      /* 0000 1000 - overflow */

/* Instructions list */
typedef enum Instructions{
    INS_NOP = 0x00,
    INS_LDA = 0x01,
    INS_LDB = 0x02,
    INS_ADD = 0x03,
    INS_SUB = 0x04,
    INS_AND = 0x05,
    INS_OR  = 0x06,
    INS_XOR = 0x07,
    INS_INC = 0x08,
    INS_DEC = 0x09,
    INS_JMP = 0x0a,
    INS_OUT = 0x0b,
    INS_STA = 0x0c,
    INS_STB = 0x0d,
    INS_JZ = 0x0e,
    INS_HLT = 0xFF
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
    bool is_halted;
    uint8_t status_flags;
    uint8_t stack_pointer;
}cpu;


void cpu_unit(cpu* current_cpu);

#endif