/*
Exercise 7-8.

Write a program to print a set of files, starting each new one on a new page,
with a title and a running page count for each file.
*/

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#define LINES_PER_PAGE  50
#define LINE_LEN        80
#define LINEBUF_SIZE    1000

int main(int argc, const char *argv[]) {
    const char **fnames;
    char linebuf[LINEBUF_SIZE];
    int  page, line;
    bool noEOF;
    FILE *fp;

    if (argc == 1) {
        printf("Usage: print_files FILE1 FILE2 ...\n");
        return 0;
    }

    for (fnames = argv+1; (fp = fopen(*fnames, "r")) != NULL; fnames++) {
        /* print file */
        noEOF = fgets(linebuf, LINEBUF_SIZE, fp);
        for (page = 1; noEOF; page++) {
            /* print title and page number */
            printf("%s %*d\n\n", *fnames, (int)(LINE_LEN - strlen(*fnames)), page);
            printf("%s", linebuf);
            /* print lines */
            line = 3;
            for (; (noEOF = fgets(linebuf, LINEBUF_SIZE, fp)) && line < LINES_PER_PAGE; line++)
                printf("%s", linebuf);
            /* pad with empty lines */
            for (; line < LINES_PER_PAGE; line++)
                putchar('\n');
            /* print "end-of-page" marker */
            printf("\x1b[35m" "[end-of-page]\n" "\x1b[0m");
        }
    }
    return 0;
}
