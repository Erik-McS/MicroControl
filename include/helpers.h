#ifndef HELPERS_H
#define HELPERS_H

#include "asm.h"

#include <string.h>

int c_stringcpy(char *dest, const char *src, size_t dest_size);
int is_mnemonic(const char *str, const Instruction *inst_table, size_t table_size);


#endif
