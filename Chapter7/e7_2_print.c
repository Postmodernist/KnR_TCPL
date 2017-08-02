/*
Exercise 7-2.

Write a program that will print arbitrary input in a sensible way. As a
minimum, it should print non-graphic characters in octal or hexadecimal
according to local custom, and break long text lines.
*/

#include <stdio.h>
#include <ctype.h>

#define LINE_WRAP_LEN 50

int main(void) {
    int c, len;

    len = 0;
    while ((c = getchar()) != EOF)
        if (c == '\n' || (len >= LINE_WRAP_LEN && !isgraph(c))) {
            putchar('\n');
            len = 0;
        } else if (isprint(c)) {
            putchar(c);
            len++;
        } else {
            printf("\\x%2.2x", c);
            len += 4;
        }
}
