/*
Print each line of file that contains a particular pattern; patterns are
highlighted. A very light version of grep.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 1024    /* maximum input line length */

char *getline_n(FILE*, char*, int);
int  strindex(char*, char*);


/* Find all lines matching pattern. */
int main(int argc, char *argv[]) {
    char *stmp, *p, s[MAXLINE];
    int i;
    FILE *f;

    if (argc < 3) {
        printf("Return lines containing a pattern.\n\n"
               "Usage:  grep PATTERN FILE\n\n");
        return 0;
    }

    p = argv[1];
    f = fopen(argv[2], "r");

    if (f == NULL) {
        fprintf(stderr, "Can't open '%s'.\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    while (getline_n(f, s, MAXLINE)) {
        stmp = s;
        while ((i = strindex(stmp, p)) >= 0) {
            for (; i > 0; i--)
                putchar(*stmp), stmp++; // string before pattern
            printf("\x1b[91m" "%s" "\x1b[0m", p); // colored pattern
            stmp += strlen(p);
        }
        if (stmp > s)
            printf("%s", stmp); // the rest of the string
    }

    fclose(f);
    return 0;
}


/* Read lines from file and stores them to string. Return pointer to a string, or NULL. */
char *getline_n(FILE *f, char s[], int n) {
    int c, i;
    for (i = 0; i < n - 1  &&  (c = fgetc(f)) != EOF  &&  c != '\n'; i++)
        s[i] = c;
    if (c == '\n')
        s[i++] = c;
    s[i] = '\0';
    return !i && c == EOF ? NULL : s;
}


/* Returns index of a pattern in a string, or -1. */
int strindex(char s[], char p[]) {
    int i, j;
    int p_len = strlen(p), len = strlen(s) - p_len;
    if (len < 0)
        return -1;
    for (i = 0; i <= len; i++) {
        for (j = 0; j < p_len  &&  s[i+j] == p[j]; j++);
        if (j == p_len)
            return i;
    }
    return -1;
}
