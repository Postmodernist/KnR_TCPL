/*
Exercise 5-11.

Modify the programs entab and detab (written as exercises in Chapter 1) to
accept a list of tab stops as arguments. Use the default tab settings if
there are no arguments.

Exercise 5-12.

Extend entab and detab to accept the shorthand

    entab -m +n

to mean tab stops every n columns, starting at column m. Choose convenient
(for the user) default behavior.
*/

#include <stdio.h>
#include <stdlib.h>

#define MAXLINE     1000
#define TABSTOP     4
#define ATOUL(x)    (strtoul((x), NULL, 10))


/* Read a line from stdin into s, return length. */
size_t lineread(char *s, size_t lim) {
    int c;
    size_t i;

    for (i = 0; (i < lim - 1) && ((c = getchar()) != EOF) && (c != '\n'); i++)
        s[i] = c;
    if (c == '\n')
        s[i++] = c;
    s[i] = '\0';
    return i;
}


/* get next tabstop index and distance to it */
#define UPDATE_TABSTOP { \
    while (j >= t) \
        t += *tabl ? ATOUL(*(tabl++)) : TABSTOP; \
    k = t - j; \
}


int main(int argc, const char *argv[]) {
    size_t len;     /* buffer length */
    size_t i;       /* buffer index */
    size_t j;       /* column index */
    size_t t;       /* tabstop index */
    int    k;       /* #columns to next tabstop */
    int    n;       /* #spaces */
    char   line[MAXLINE];
    const char **tabl;

    while ((len = lineread(line, MAXLINE)) > 0) {
        t = 0;
        tabl = argv+1;        
        for (i = 0, j = 0; i < len; i++, j++)

            if (line[i] == ' ') {
                UPDATE_TABSTOP
                /* count blanks */
                for (n = 0; n < k && line[i + n] == ' '; n++)
                    ;

                if (n == 1) {
                    putchar(' ');       /* both fit: use single space */

                } else if (n == k) {
                    i += n - 1;
                    j += n - 1;
                    putchar('\t');      /* replace spaces with tab */

                } else if (line[i + n] == '\t') {
                    i += n;
                    j += k - 1;
                    putchar('\t');      /* replace blanks+tab with tab */

                } else {
                    i += n - 1;
                    j += n - 1;
                    for (; n; n--)      /* print stray spaces */
                        putchar(' ');
                }

            } else if (line[i] == '\t') {
                UPDATE_TABSTOP
                j += k - 1;
                putchar('\t');          /* print tab */
            
            } else {
                putchar(line[i]);       /* print regular character */
            }
    }

    return 0;
}
