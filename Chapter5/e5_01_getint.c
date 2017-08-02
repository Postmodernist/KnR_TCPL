/*
Exercise 5-1.

As written, getint treats a + or - not followed by a digit as a valid
representation of zero. Fix it to push such a character back on the input.
*/

#include <stdio.h>
#include <ctype.h>

int  getch(void);
void ungetch(int);


/* Get next integer from input into *pn */
int getint(int *pn) {
    int c, sign;

    while (isspace(c = getch()));  /* skip white space */
    if (!isdigit(c) && c != EOF && c != '+' && c != '-') {
        ungetch(c);  /* it's not a number */
        return 0;
    }
    sign = (c == '-') ? -1 : 1;
    if (c == '+' || c == '-')
        if (!isdigit(c = getch()) && c != EOF) {
            ungetch(c);
            return 0;
        }
    for (*pn = 0; isdigit(c); c = getch())
        *pn = 10 * *pn + (c - '0');
    *pn *= sign;
    if (c != EOF)
        ungetch(c);
    return c;
}


int main() {
    int n, res;

    res = getint(&n);
    printf("%d %d\n", res, n);
}


/* ---------------------------------------------------------------------------- */
/* Getch / ungetch */

#define BUFSIZE 4
char buf[BUFSIZE];
int  bufp = 0;

int getch(void) {
    return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c) {
    if (bufp < BUFSIZE)
        buf[bufp++] = c;
    else
        fprintf(stderr, "ungetch: Too many characters\n");
}
