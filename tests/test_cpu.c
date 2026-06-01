#include <stdio.h>
#include <assert.h>
#include "../include/cpu.h"

void test_cpu_sta(){
    printf("Running test_cpu_sta...\n");
    /* cpu setup */
    cpu test_cpu ={0};
    test_cpu.reg_a = 0x42;
    test_cpu.ram[0] = INS_STA;
    test_cpu.ram[1] = 0x50;
    test_cpu.program_counter = 0;

    /* on cycle */
    cpu_unit(&test_cpu);
    /* correct value at address */
    assert(test_cpu.ram[0x50] == 0x42);
    /* PC +2 after cycle*/
    assert(test_cpu.program_counter == 2);
    printf("  -> PASS\n");
}

void test_cpu_jz() {
    printf("Running test_cpu_jz...\n");

    /* --- PATH 1: Flag is set -> Should Jump --- */
    cpu cpu_should_jump = {0};
    cpu_should_jump.status_flags |= FLAG_ZERO; 
    cpu_should_jump.ram[0] = INS_JZ;
    cpu_should_jump.ram[1] = 0x20;             
    cpu_should_jump.program_counter = 0;

    cpu_unit(&cpu_should_jump);
    // PC should have jumped straight to 0x20
    assert(cpu_should_jump.program_counter == 0x20);

    /* --- PATH 2: Flag is clear -> Should NOT Jump --- */
    cpu cpu_should_fail = {0};
    cpu_should_fail.status_flags &= ~FLAG_ZERO; // Force Zero Flag OFF
    cpu_should_fail.ram[0] = INS_JZ;
    cpu_should_fail.ram[1] = 0x20;              
    cpu_should_fail.program_counter = 0;

    cpu_unit(&cpu_should_fail);
    // PC should have just skipped past the operand, ending up at index 2
    assert(cpu_should_fail.program_counter == 2);

    printf("  -> PASS\n");
}

int main() {
    printf("=== Starting CPU Test Suite ===\n");
    test_cpu_sta();
    test_cpu_jz();
    printf("=== All CPU tests passed successfully! ===\n");
    return 0;
}
