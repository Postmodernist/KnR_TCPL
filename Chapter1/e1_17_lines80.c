/*
Exercise 1-17.

Write a program to print all input lines that are longer that 80 characters.
*/

#include <stdio.h>

#define MAXLINE 1000    /* maximum input line size */


/* Read a line from stdin into s, return length. */
int lineread(char *s, int lim) {
    int c, i;

    for (i = 0; (i < lim - 1) && ((c = getchar()) != EOF) && (c != '\n'); ++i)
        s[i] = c;

    if (c == '\n')
        s[i++] = c;

    s[i] = '\0';

    return i;
}


int main() {
    int  len;
    char line[MAXLINE];

    while ((len = lineread(line, MAXLINE)) > 0) {
        if (len > 80) {
            printf("%s", line);

            /* read the rest of the line if needed */
            while (line[len - 1] != '\n') {
                len = lineread(line, MAXLINE);
                printf("%s", line);
            }
        }
    }

    return 0;
}
