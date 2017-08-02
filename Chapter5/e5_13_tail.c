/*
Exercise 5-13.

Write the program tail, which prints the last n lines of its input. By
default, n is 10, let us say, but it can be changed by an optional argument,
so that

    tail -n

prints the last n lines. The program should behave rationally no matter how
unreasonable the input or the value of n. Write the program so it makes the
best use of available storage; lines should be stored as in the sorting program
of Section 5.6, not in a two-dimensional array of fixed size.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_BUF_SIZE           1024
#define MAX_LINES               1024
#define LINES_DEF               10      /* default lines number */
#define PNEXT(ptr, beg, sz)     ptr + 1 == beg + sz ? beg : ptr + 1
#define PPREV(ptr, beg, sz)     ptr == beg ? beg + sz - 1 : ptr - 1

int main(int argc, const char *argv[]) {
    const char *fname = argv[1];
    FILE *fp;
    int  linesno = LINES_DEF;  
    char buf[LINE_BUF_SIZE];

    /* process arguments */
    if (argc < 2) {
        printf("Print 10 or N last lines of FILE.\n"
               "Usage:  tail [-N] FILE\n");
        exit(EXIT_SUCCESS);
    } else if (argc == 2) {
        fname = argv[1];
    } else if (argc == 3 && argv[1][0] == '-') {
        linesno = atoi(argv[1] + 1);
        if (linesno > MAX_LINES)
            linesno = MAX_LINES;
        fname = argv[2];
    } else {
        printf("Error: invalid arguments\n");
        exit(EXIT_FAILURE);
    }

    /* allocate array for line pointers */
    char *lines[linesno];
    char **lp = lines;
    memset(lines, 0, linesno * sizeof(char *));

    /* open input file */
    if ((fp = fopen(fname, "r")) == NULL) {
        printf("Error: can't open '%s'.\n", fname);
        exit(EXIT_FAILURE);
    }

    /* read lines */
    for (; fgets(buf, LINE_BUF_SIZE, fp); lp = PNEXT(lp, lines, linesno)) {
        if (*lp)
            free(*lp);
        *lp = malloc(strlen(buf) + 1);
        strcpy(*lp, buf);
    }    

    /* print lines */
    char **pp = PPREV(lp, lines, linesno);
    for (; lp != pp; lp = PNEXT(lp, lines, linesno))
        if (*lp) {
            printf("%s", *lp);
            free(*lp);
        }
    if (*lp) {
        printf("%s", *lp);
        free(*lp);
    }
    fclose(fp);
    exit(EXIT_SUCCESS);
}
