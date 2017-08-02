/* ---------------------------------------------------------------------------- */
/* Recursive descent parser. Converts C declarations to words. */
/* ---------------------------------------------------------------------------- */

#include <stdbool.h>
#include <stdio.h>
#include "defs.h"

char out[OUTPUT_LEN];
bool read_EOF = false;

int main() {
    while (!read_EOF) {
        if (tokens_read() && declaration())
            printf("%s\n", out);
        out[0] = '\0';
        tokens_free();
    }
}
