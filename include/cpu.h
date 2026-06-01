#ifndef CPU_H
# define CPU_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_RAM_SIZE 256
#define DEBUG 1
#define FLAG_ZERO (1U<<0) /* 0000 0001 */

typedef enum Instructions{
    INS_NOP = 0x00,
    INS_LDA = 0x01,
    INS_LDB = 0x02,
    INS_ADD = 0x03,
    INS_JMP = 0x04,
    INS_OUT = 0x05,
    INS_STA = 0x06,
    INS_STB = 0x07,
    INS_JZ = 0x08,
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
}cpu;


void cpu_unit(cpu* current_cpu);

#endif