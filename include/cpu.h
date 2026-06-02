#ifndef CPU_H
# define CPU_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_RAM_SIZE 256
#define MAX_REGISTER_SIZE 4
#define DEBUG 1


/* Routing */
/* [ I ] [ M ] [ x ] [ x ] [ Dest Bit 1 ] [ Dest Bit 0 ] [ Source Bit 1 ] [ Source Bit 0 ]*/
/* immediate flag test macro */
#define ISIMMEDIATE(byte) ( ((byte) & 0x80) !=0)
#define ISMEMORY(byte) ( ((byte) & 0x40) !=0)
/* Masks */
# define DESTINATION_MASK 0x0c
#define SOURCE_MASK 0x03


/* Status flags */
#define FLAG_ZERO (1U<<0)   /* 0000 0001 - equals 0*/
#define FLAG_NEG (1U<<1)    /* 0000 0010 - is negative */
#define FLAG_C (1U<<2)      /* 0000 0100 - has carry */
#define FLAG_O (1U<<3)      /* 0000 1000 - overflow */


/* Instructions list */
typedef enum Instructions{
    INS_NOP = 0x00,
    INS_LD = 0x01,
    INS_ADD = 0x02,
    INS_SUB = 0x03,
    INS_AND = 0x04,
    INS_OR  = 0x05,
    INS_XOR = 0x06,
    INS_INC = 0x07,
    INS_DEC = 0x08,
    INS_JMP = 0x09,
    INS_OUT = 0x0a,
    INS_ST = 0x0b,
    INS_JZ = 0x0c,
    INS_HLT = 0xFF
}Instructions;

/* Defines the cpu*/
typedef struct cpu
{
    /* Array for ram */
    uint8_t ram[MAX_RAM_SIZE];
    /* Registers file - 0 to 3, 4 registers */
    uint8_t registers[MAX_REGISTER_SIZE];
    /* PC */
    uint8_t program_counter;
    bool is_halted;
    uint8_t status_flags;
    uint8_t stack_pointer;
}cpu;


void cpu_unit(cpu* current_cpu);
void execute_arithmetic(cpu* current_cpu, uint8_t alu_operation);
void execute_load(cpu* current_cpu);
void execute_store(cpu* current_cpu);
void execute_unary(cpu* current_cpu, uint8_t alu_operation);
void execute_jmp(cpu* current_cpu);
void execute_jz(cpu* current_cpu);
#endif