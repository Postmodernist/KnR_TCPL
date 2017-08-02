#include <stdio.h>
#include <stdlib.h>

/* Get n bits from position p */
unsigned getbits(unsigned x, int p, int n) {
    return (x >> (p + 1 - n)) & ~(~0 << n);
}

int main() {
    printf("%u\n", getbits(12345, 5, 3));  /* 7 */
}
