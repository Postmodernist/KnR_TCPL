/* Pseudo-random number generator. */

#include <stdio.h>
#include <stdlib.h>

unsigned long int next = 1;

/* Return pseudo-random integer on 0..32767 */
int rand_n(void) {
    next = next * 1103515245 + 12345;
    return (unsigned int)(next / 65536) % 32768;
}

/* Set seed for rand_n() */
void randseed(unsigned int seed) {
    next = seed;
}

int main() {
    randseed(12345);
    printf("%d\n", rand_n());
}
