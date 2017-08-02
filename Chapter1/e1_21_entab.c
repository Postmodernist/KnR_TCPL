/*
Exercise 1-21.

Write a program entab that replaces strings of blanks by the minimum number of tabs
and blanks to achieve the same spacing. Use the same tab stops as for detab. When
either a tab or a single blank would suffice to reach a tab stop, which should be
given preference?
*/

#include <stdio.h>

#define MAXLINE 1000
#define TABSTOP 4
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
    int    n;       /* #spaces */
    char   line[MAXLINE];

    k = -1;
    while ((len = lineread(line, MAXLINE)) > 0)
        for (i = 0, j = 0; i < len; i++, j++)

            if (line[i] == ' ') {
                k = NEXT_TAB(j);
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
                    putchar('\t');      /* replace mixed blanks with tab */

                } else {
                    i += n - 1;
                    j += n - 1;
                    for (; n; n--)      /* print stray spaces */
                        putchar(' ');
                }

            
            } else if (line[i] == '\t') {
                k = NEXT_TAB(j);
                j += k - 1;
                putchar('\t');          /* print tab */
            
            } else {
                putchar(line[i]);       /* print regular character */
            }

    return 0;
}
