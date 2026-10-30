#include "helpers.h"
#include "asm.h"

/* Copy the complete string only when it fits, including its terminating '\0'.
 * Returns 0 on success and -1 for invalid arguments or insufficient space.
 */
int c_stringcpy(char *dest, const char *src, size_t dest_size) {
    if (dest == NULL || src == NULL || dest_size == 0) {
        return -1;
    }

    size_t source_size = strlen(src);
    /* Reject truncation so the caller can report an invalid token. */
    if (source_size >= dest_size) {
        dest[0] = '\0';
        return -1;
    }

    memcpy(dest, src, source_size + 1);
    return 0;
}

/* Search the instruction table for an exact mnemonic match.
 * Returns 1 when found and 0 otherwise.
 */
int is_mnemonic(const char *str, const Instruction *inst_table, size_t table_size) {
    if (str == NULL || inst_table == NULL) {
        return 0;
    }

    /* The table is small, so a straightforward linear search is sufficient. */
    for (size_t i = 0; i < table_size; i++) {
        if (strcmp(str, inst_table[i].instruction) == 0) {
            return 1;
        }
    }
    return 0;
}
