/*
Exercise 1-19.

Write a function reverse(s) that reverses the character string s. Use it to
write a program that reverses its input a line at a time.
*/

#include <stdio.h>

#define MAXLINE 1000


/* Reverses string s in place. */
char *reverse(char s[], size_t len) {
    int c, i;

    if (s[--len] == '\n')
        len--;

    for (i = 0; i < len; i++, len--) {
        c = s[i];
        s[i] = s[len];
        s[len] = c;
    }

    return s;
}


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

    while ((len = lineread(line, MAXLINE)) > 0)
        printf("%s", reverse(line, len));

    return 0;
}
