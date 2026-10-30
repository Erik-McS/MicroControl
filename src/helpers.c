#include "helpers.h"
#include "asm.h"

int c_stringcpy(char *dest, const char *src, size_t dest_size) {
    if (dest == NULL || src == NULL || dest_size == 0) {
        return -1; // Invalid parameters
    }

    size_t i;
    for (i = 0; i < dest_size - 1 && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0'; // Null-terminate the destination string

    return 0; // Success
}

int is_mnemonic(const char* str, Instruction* inst_table, size_t table_size) {

    /* Iterate through the instruction table to find a matching mnemonic. */
    for(int i=0; i < table_size; i++){
        if(strcmp(str, inst_table[i].opcode) == 0){
            return 1; // Found a matching mnemonic
        }
    }
    return 0; // No matching mnemonic found
}
