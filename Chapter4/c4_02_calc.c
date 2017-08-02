#include <stdio.h>
#include <stdlib.h>

#define MAXLINE 100


/* Rudumentary calculator. */
int main() {
    double sum, atof_n(const char[]);
    char line[MAXLINE];

    sum = 0;
    while (fgets(line, MAXLINE, stdin))
        printf("\t%g\n", sum += atof(line));
    return 0;
}
