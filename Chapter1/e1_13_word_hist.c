/*
Exercise 1-13.

Write a program to print a histogram of the lengths of words in its input. It
is easy to draw the histogram with the bars horizontal; a vertical orientation
is more challenging.
*/

#include <stdio.h>
#include <stdbool.h>

#define WORD_LEN_MAX 32

/* Return true if c is a word character. */
bool iswordchar(char c) {
    return (c >= '0' && c <= '9') ||
           (c >= 'A' && c <= 'Z') ||
           (c >= 'a' && c <= 'z') ||
           (c == '_');
}

int main() {
    int c, n, i;
    int wlen[WORD_LEN_MAX] = {};

    /* count words length */
    c = getchar();
    while (c != EOF) {
        if (iswordchar(c)) {
            for (n = 0; c != EOF && iswordchar(c); n++)
                c = getchar();
            wlen[n]++;
        } else {
            while (c != EOF && !iswordchar(c))
                c = getchar();
        }
    }

    /* print out */
    printf("Word length frequencies:\n");
    for (n = 0; n < WORD_LEN_MAX; n++) {
        if (wlen[n] == 0)
            continue;
        printf("%3d ", n);
        for (i = 0; i < wlen[n]; i++)
            putchar('=');
        printf(" %d\n", wlen[n]);
    }    
}
