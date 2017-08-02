/*
Exercise 2-9.

In a two's complement number system, x &= (x-1) deletes the rightmost 1-bit in
x. Explain why. Use this observation to write a faster version of bitcount.
*/

#include <stdio.h>


/* Returns the number of 1 bits in X. */
int bitcount(unsigned x) {
    int b;

    for (b = 0; x; x &= x - 1, b++);
    return b;
}


int main() {
    printf("%d\n", bitcount(100));  /* 3 */
}
