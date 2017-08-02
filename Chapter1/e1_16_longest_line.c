/*
Exercise 1-16.

Revise the main routine of the longest-line program so it will correctly print
the length of arbitrarily long input lines, and as much as possible of the text.
*/

#include <stdio.h>
#include <stdbool.h>

#define MAXLINE 1000    /* maximum input line size */

int  lineread(char*, int);
void linecopy(char*, char*);

/* Print longest input line. */
int main() {
    int  n;                     /* number of characters read */
    int  len;                   /* current line length */
    int  max = 0;               /* maximum length so far */
    char line[MAXLINE];         /* current input line */
    char line_cand[MAXLINE];    /* first part of candidate line */
    char longest[MAXLINE];      /* longest line saved here */
    bool line_closed = true;    /* line reading finished */

    while ((n = lineread(line, MAXLINE)) > 0) {
        
        if (line_closed) {
            len = n;
            linecopy(line_cand, line);
        } else
            len += n;
        
        line_closed = line[n-1] == '\n';

        if (len > max && line_closed) {
            max = len;
            linecopy(longest, line_cand);
        }
    }

    if (max > 0) {  /* there was a line */
        if (longest[max-1] == '\n')
            printf("%sLength: %d\n", longest, max);
        else
            printf("%s[...]\nLength: %d\n", longest, max);
    }

    return 0;
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


/* Copy 'from' into 'to'; assume 'to' is big enough. */
void linecopy(char *to, char *from) {
    int i = 0;
    
    while ((to[i] = from[i]) != '\0')
        ++i;
}
