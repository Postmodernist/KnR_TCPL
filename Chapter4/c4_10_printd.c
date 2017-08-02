#include <stdio.h>
#include <stdlib.h>

/* Print n in decimal. */
void printd_r(int n) {
    if (n < 0) {
        putchar('-');
        n = -n;
    }
    if (n / 10)
        printd_r(n / 10);
    putchar(n % 10 + '0');
}

int main(int argc, const char *argv[]) {
    if (argc == 2)
        printd_r(atoi(argv[1]));
    putchar('\n');
}
