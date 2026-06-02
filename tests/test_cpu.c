#include <stdio.h>
#include <assert.h>
#include "../include/cpu.h"
#include "../include/alu.h" // Depending on how your headers are linked

/* ---------------------------------------------------------
 * TEST 1: Load Immediate to Register
 * Assembly: LD R1, $0x55
 * Bytes: [Opcode] [Routing] [Data]
 * ---------------------------------------------------------*/
void test_cpu_ld_immediate() {
    printf("Running test_cpu_ld_immediate...\n");

    cpu test_cpu = {0};
    test_cpu.ram[0] = INS_LD;
    /* Routing: I=1, M=0, Dest=R1 (0x04), Src=0 -> 0x80 | 0x04 = 0x84 */
    test_cpu.ram[1] = 0x84; 
    test_cpu.ram[2] = 0x55;
    test_cpu.program_counter = 0;

    cpu_unit(&test_cpu);

    /* R1 should hold the data, and PC should have stepped 3 bytes */
    assert(test_cpu.registers[1] == 0x55);
    assert(test_cpu.program_counter == 3);
    
    printf("  -> PASS\n");
}

/* ---------------------------------------------------------
 * TEST 2: Store Register to Memory (Direct)
 * Assembly: ST R2, [0x50]
 * Bytes: [Opcode] [Routing] [Address]
 * ---------------------------------------------------------*/
void test_cpu_st_memory_direct() {
    printf("Running test_cpu_st_memory_direct...\n");

    cpu test_cpu = {0};
    /* Pre-load R2 with some data */
    test_cpu.registers[2] = 0x42; 
    
    test_cpu.ram[0] = INS_ST;
    /* Routing: I=0, M=1, Dest=0, Src=R2 (0x02) -> 0x40 | 0x02 = 0x42 */
    test_cpu.ram[1] = 0x42; 
    test_cpu.ram[2] = 0x50;
    test_cpu.program_counter = 0;

    cpu_unit(&test_cpu);

    /* Data should be in RAM, and PC should have stepped 3 bytes */
    assert(test_cpu.ram[0x50] == 0x42);
    assert(test_cpu.program_counter == 3);
    
    printf("  -> PASS\n");
}

/* ---------------------------------------------------------
 * TEST 3: Arithmetic (Register to Register)
 * Assembly: ADD R1, R2
 * Bytes: [Opcode] [Routing]
 * ---------------------------------------------------------*/
void test_cpu_add_r_to_r() {
    printf("Running test_cpu_add_r_to_r...\n");

    cpu test_cpu = {0};
    /* Pre-load R1 and R2 */
    test_cpu.registers[1] = 0x05;
    test_cpu.registers[2] = 0x0A;
    
    test_cpu.ram[0] = INS_ADD;
    /* Routing: I=0, M=0, Dest=R1 (0x04), Src=R2 (0x02) -> 0x06 */
    test_cpu.ram[1] = 0x06; 
    test_cpu.program_counter = 0;

    cpu_unit(&test_cpu);

    /* 0x05 + 0x0A = 0x0F. PC should have stepped 2 bytes */
    assert(test_cpu.registers[1] == 0x0F);
    assert(test_cpu.program_counter == 2);
    
    printf("  -> PASS\n");
}

/* ---------------------------------------------------------
 * TEST 4: Unconditional Jump (Immediate)
 * Assembly: JMP $0x42
 * Bytes: [Opcode] [Routing] [Target Address]
 * ---------------------------------------------------------*/
void test_cpu_jmp_immediate() {
    printf("Running test_cpu_jmp_immediate...\n");

    cpu test_cpu = {0};
    test_cpu.ram[0] = INS_JMP;
    /* Routing: I=1, M=0 -> 0x80 */
    test_cpu.ram[1] = 0x80;
    test_cpu.ram[2] = 0x42;
    test_cpu.program_counter = 0;

    cpu_unit(&test_cpu);

    /* PC should be entirely overwritten by the target address */
    assert(test_cpu.program_counter == 0x42);
    
    printf("  -> PASS\n");
}

/* ---------------------------------------------------------
 * TEST 5: Conditional Jump if Zero (Immediate)
 * Assembly: JZ $0x20
 * ---------------------------------------------------------*/
void test_cpu_jz_immediate() {
    printf("Running test_cpu_jz_immediate...\n");

    /* --- PATH 1: Flag is set -> Should Jump --- */
    cpu cpu_should_jump = {0};
    cpu_should_jump.status_flags |= FLAG_ZERO; 
    cpu_should_jump.ram[0] = INS_JZ;
    cpu_should_jump.ram[1] = 0x80; // I=1, M=0             
    cpu_should_jump.ram[2] = 0x20;             
    cpu_should_jump.program_counter = 0;

    cpu_unit(&cpu_should_jump);
    // PC should have jumped straight to 0x20
    assert(cpu_should_jump.program_counter == 0x20);

    /* --- PATH 2: Flag is clear -> Should NOT Jump --- */
    cpu cpu_should_fail = {0};
    cpu_should_fail.status_flags &= ~FLAG_ZERO; // Force Zero Flag OFF
    cpu_should_fail.ram[0] = INS_JZ;
    cpu_should_fail.ram[1] = 0x80; // I=1, M=0               
    cpu_should_fail.ram[2] = 0x20;              
    cpu_should_fail.program_counter = 0;

    cpu_unit(&cpu_should_fail);
    /* * Because I=1, the skip logic needs to clear the opcode (1), 
     * the routing byte (1), and the address byte (1). PC should be 3.
     */
    assert(cpu_should_fail.program_counter == 3);

    printf("  -> PASS\n");
}

int main() {
    printf("=== Starting MicroControl CPU Test Suite ===\n");
    test_cpu_ld_immediate();
    test_cpu_st_memory_direct();
    test_cpu_add_r_to_r();
    test_cpu_jmp_immediate();
    test_cpu_jz_immediate();
    printf("=== All CPU tests passed successfully! ===\n");
    return 0;
}