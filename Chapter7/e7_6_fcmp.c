/*
Exercise 7-6.

Write a program to compare two files, printing the first line where they
differ.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STR_MAXLEN 1024

const char *prog;  /* program name for errors */


int main(int argc, char const *argv[]) {
    void file_open(FILE **, const char *);
    FILE *fp1, *fp2;
    char s1[STR_MAXLEN], s2[STR_MAXLEN], *tmp1, *tmp2;
    int  line;

    prog = argv[0];
    
    if (argc != 3) {
        fprintf(stderr, "%s: expected exactly 2 arguments\n", prog);
        exit(EXIT_FAILURE);
    }

    file_open(&fp1, argv[1]);
    file_open(&fp2, argv[2]);

    line = 0;
    do {
        line++;
        tmp1 = fgets(s1, STR_MAXLEN, fp1);
        tmp2 = fgets(s2, STR_MAXLEN, fp2);
        if (tmp1 == NULL && tmp2 == NULL)
            break;
    } while (!strcmp(s1, s2));

    if (tmp1 || tmp2) {
        printf("difference at line %d:\n", line);
        printf("\x1b[31m" "%s:\n" "\x1b[0m", argv[1]);
        if (tmp1)
            printf("%s", s1);
        else 
            printf("EOF");
        printf("\x1b[31m" "%s:\n" "\x1b[0m", argv[2]);
        if (tmp2)
            printf("%s", s2);
        else 
            printf("EOF\n");
    }
    
    fclose(fp1);
    fclose(fp2);
    exit(EXIT_SUCCESS);
}


void file_open(FILE **fp, const char *fname) {
    if ((*fp = fopen(fname, "r")) == NULL) {
        fprintf(stderr, "%s: can't open '%s'\n", prog, fname);
        exit(EXIT_FAILURE);
    }
}
