#ifndef HELPERS_H
#define HELPERS_H

#include <string.h>

int c_stringcpy(char *dest, const char *src, size_t dest_size);
int is_mnemonic(const char* str, Instruction* inst_table, size_t table_size);


#endif