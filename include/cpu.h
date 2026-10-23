#ifndef CPU_H
# define CPU_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_RAM_SIZE 256
#define MAX_REGISTER_SIZE 4
#define DEBUG 1 /* Enables progress messages guarded by #if DEBUG. */


/* Routing byte: [I][M][unused][unused][destination:2 bits][source:2 bits].
 * I/M select the operand mode:
 *   00: source register
 *   10: next byte as an immediate value
 *   01: RAM at the address in the next byte
 *   11: RAM at the address held in the source register
 */
#define ISIMMEDIATE(byte) ( ((byte) & 0x80) !=0)
#define ISMEMORY(byte) ( ((byte) & 0x40) !=0)
/* Extract the 2-bit destination register (bits 3-2) and source register (bits 1-0). */
# define DESTINATION_MASK 0x0c
#define SOURCE_MASK 0x03


/* Bit positions in status_flags. */
#define FLAG_ZERO (1U<<0)   /* 0000 0001 - equals 0*/
#define FLAG_NEG (1U<<1)    /* 0000 0010 - is negative */
#define FLAG_C (1U<<2)      /* 0000 0100 - has carry */
#define FLAG_O (1U<<3)      /* 0000 1000 - overflow */


/* Machine opcodes fetched and dispatched by the CPU. */
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

/* Complete state of the emulated 8-bit CPU. */
typedef struct cpu
{
    /* Byte-addressable memory; the program is stored here as well. */
    uint8_t ram[MAX_RAM_SIZE];
    /* Four general-purpose 8-bit registers, indexed 0 through 3. */
    uint8_t registers[MAX_REGISTER_SIZE];
    /* Address of the next instruction or instruction operand to process. */
    uint8_t program_counter;
    bool is_halted;         /* Set when execution stops or encounters a fault. */
    uint8_t status_flags;   /* Bit field containing FLAG_ZERO, FLAG_NEG, FLAG_C, FLAG_O. */
    uint8_t stack_pointer;  /* 8-bit stack location. */
}cpu;


/* Fetch and execute the instruction at the current program counter. */
void cpu_unit(cpu* current_cpu);

/* Execute an ALU operation using the routing byte and update result flags. */
void execute_arithmetic(cpu* current_cpu, uint8_t alu_operation);

/* Load from the routed register or memory operand into the destination register. */
void execute_load(cpu* current_cpu);

/* Store a register to the routed memory address or destination register. */
void execute_store(cpu* current_cpu);

/* Execute increment/decrement on the routed register. */
void execute_unary(cpu* current_cpu, uint8_t alu_operation);

/* Jump unconditionally to an immediate or register-held address. */
void execute_jmp(cpu* current_cpu);

/* Jump to an immediate or register-held address only when ZERO is set. */
void execute_jz(cpu* current_cpu);
#endif