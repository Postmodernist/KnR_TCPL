/*
Exercise 1-9.

Write a program to copy its input to output, replacing each string of one or
more blanks by a single blank.
*/

#include <stdio.h>
#include <stdbool.h>

/* Return true if c is blank. */
bool isblank_n(char c) {
    return c == ' ' || c == '\t';
}

int main() {
    int c;

    c = getchar();
    while (c != EOF) {
        putchar(c);
        if (isblank_n(c))
            while(isblank_n(c))
                c = getchar();
        else
            c = getchar();
    }
}
