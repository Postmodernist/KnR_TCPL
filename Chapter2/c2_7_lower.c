#include <stdio.h>

/* Convert c to lower case; ASCII only. */
int lower_n(int c) {
    if (c >= 'A' && c <= 'Z')
        return c + 'a' - 'A';
    else
        return c;
}

int main() {
    putchar(lower_n('C'));
}
