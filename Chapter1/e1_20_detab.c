/*
Exercise 1-20.

Write a program detab that replaces tabs in the input with the proper number of
blanks to space to the next tab stop. Assume a fixed set of tab stops, say every
n columns. Should n be a variable or a symbolic parameter?
*/

#include <stdio.h>

#define MAXLINE 1000
#define TABSTOP 8
#define NEXT_TAB(x) (TABSTOP - (x) % TABSTOP)


/* Read a line from stdin into s, return length. */
size_t lineread(char *s, size_t lim) {
    int c;
    size_t i;

    for (i = 0; (i < lim - 1) && ((c = getchar()) != EOF) && (c != '\n'); ++i)
        s[i] = c;

    if (c == '\n')
        s[i++] = c;

    s[i] = '\0';

    return i;
}


int main() {
    size_t len;     /* buffer length */
    size_t i;       /* buffer index */
    size_t j;       /* column index */
    int    k;       /* #columns to next tabstop */
    char   line[MAXLINE];

    while ((len = lineread(line, MAXLINE)) > 0)
        for (i = 0, j = 0; i < len; i++, j++)
            if (line[i] != '\t')
                putchar(line[i]);
            else {
                k = NEXT_TAB(j);
                j += k - 1;
                for (; k; k--)
                    putchar(' ');
            }

    return 0;
}
