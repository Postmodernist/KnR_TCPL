/*
Exercise 8-1.

Rewrite the program cat from Chapter 7 using read, write, open and close
instead of their standard library equivalents. Perform experiments to
determine the relative speeds of the two versions.
*/

#include <fcntl.h>      /* open, creat */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     /* read, write, close */

#define STDIN 0
#define STDOUT 1

/* Print and error message and die. */
void error(char *fmt, ...) {
    va_list args;

    va_start(args, fmt);
    fprintf(stderr, "\x1b[91m" "error: " "\x1b[0m");
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
    exit(EXIT_FAILURE);
}


/* Copy src to dst. */
void filecopy(int src, int dst) {
    char buf[BUFSIZ];
    int n;

    while ((n = read(src, buf, BUFSIZ)) > 0)
        if (write(dst, buf, n) != n)
            error("filecopy: writing failed to file with descriptor %d", dst);
}


/* Concatenate files. */
int main(int argc, char const *argv[]) {
    int fd;

    if (argc == 1)
        filecopy(STDIN, STDOUT);
    else
        while (--argc > 0)
            if ((fd = open(*++argv, O_RDONLY, 0)) == -1)
                error("cat: can't open '%s'", *argv);
            else {
                filecopy(fd, STDOUT);
                close(fd);
            }
    exit(EXIT_SUCCESS);
}
