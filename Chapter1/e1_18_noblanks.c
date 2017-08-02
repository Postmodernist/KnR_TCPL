/*
Exercise 1-18.

Write a program to remove trailing blanks and tabs from each line of input, and
to delete entirely blank lines.
*/

#include <stdio.h>
#include <stdbool.h>

#define MAXLINE 1000    /* maximum input line size */

/* Return true if c is blank. */
bool isblank_n(char c) {
    return c == ' ' || c == '\t';
}

/* Read a line from stdin into s, return length. */
int lineread(char *s, int lim) {
    int c, i;

    for (i = 0; (i < lim - 1) && ((c = getchar()) != EOF) && (c != '\n'); ++i)
        s[i] = c;

    /* remove trailing blanks */
    while (isblank_n(s[--i]))
        ;
    i++;

    if (c == '\n')
        s[i++] = c;

    s[i] = '\0';

    return i;
}

int main() {
    int  len;
    char line[MAXLINE];

    while ((len = lineread(line, MAXLINE)) > 0) {
        if (len > 1)
            printf("%s", line);
    }

    return 0;
}
