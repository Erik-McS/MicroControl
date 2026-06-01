#include <stdio.h>
#include <assert.h>
#include "../include/alu.h"

/* * TEST: Verify that the ALU correctly adds two numbers 
 * without overflowing.
 */
void test_alu_add() {
    printf("Running test_alu_add...\n");
    uint8_t operand_a = 0x10; 
    uint8_t operand_b = 0x20; 
    uint8_t result = alu_compute(ALU_ADD, operand_a, operand_b);
    assert(result == 0x30); 
    printf("  -> PASS\n");
}

int main() {
    printf("=== Starting ALU Test Suite ===\n");
    test_alu_add();
    printf("=== All ALU tests passed successfully! ===\n");
    return 0;
}