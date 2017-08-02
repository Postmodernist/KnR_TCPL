/*
Exercise 1-8.

Write a program to count blanks, tabs, and newlines.
*/

#include <stdio.h>
#include <stdbool.h>

/* Return true if str contains chr. */
bool contains(char *str, char chr) {
    for (; *str; str++)
        if (*str == chr)
            return true;
    return false;
}

int main() {
    char symbols[3] = { '\n', '\t', ' ' };
    int c, n;

    n = 0;
    while ((c = getchar()) != EOF)
        if (contains(symbols, c))
            n++;
    printf("%d\n", n);
}
