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

int main() {
    printf("=== Starting CPU Test Suite ===\n");
    test_cpu_sta();
    printf("=== All CPU tests passed successfully! ===\n");
    return 0;
}
