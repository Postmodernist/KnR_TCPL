#include <stdio.h>

int power(int base, int n);

/* Test power function. */
int main() {
    int i;

    for (i = 0; i < 10; ++i)
        printf("%d %5d %7d\n", i, power(2, i), power(-3, i));
    return 0;
}

/* Raise base to n-th power; n >= 0; version 2 */
int power(int base, int n) {
    int p;

    for (p = 1; n > 0; --n)
        p = p * base;
    return p;
}
