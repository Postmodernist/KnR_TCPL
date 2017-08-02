/*
Exercise 1-14.

Write a program to print a histogram of the frequencies of different characters
in its input.
*/

#include <stdio.h>

#define NUM_CHARS 128

int main() {
    int c, i, j;
    int nchar[NUM_CHARS];

    for (i = 0; i < NUM_CHARS; ++i)
        nchar[i] = 0;

    while ((c = getchar()) != EOF)
        ++nchar[c];

    printf("Character frequencies:\n");
    for (i = 0; i < NUM_CHARS; ++i) {
        if (!nchar[i] || i == ' ' || i == '\n')
            continue;  /* skip spaces and newlines */

        if (i <= 32 || i == 127)
            printf("#%02X ", i);    /* control characters */
        else
            printf(" %c  ", i);     /* regular characters */

        for (j = 0; j < nchar[i]; ++j)
            putchar('=');

        printf(" %d\n", nchar[i]);
    }
}
