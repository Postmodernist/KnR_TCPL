/*
Exercise 7-1.

Write a program that converts upper case to lower or lower case to upper,
depending on the name it is invoked with, as found in argv[0].
*/

#include <stdio.h>
#include <string.h>

int main(int argc, const char *argv[]) {
    int offset, low, high, c;

    if (strstr(argv[0], "upper")) {
        /* to upper */
        offset = -32;
        low = 96;
        high = 123;
    } else {
        /* to lower */
        offset = 32;
        low = 64;
        high = 91;
    }

    while ((c = getchar()) != EOF)
        putchar(c > low && c < high ? c + offset : c);
}
