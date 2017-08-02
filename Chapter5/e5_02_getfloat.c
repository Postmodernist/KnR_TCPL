/*
Exercise 5-2.

Write getfloat, the floating-point analog of getint. What type does getfloat
return as its function value?
*/

#include <stdio.h>
#include <ctype.h>

int  getch(void);
void ungetch(int);

/* Get next floating point number from input into *pn. */
int getfloat(double *pn) {
    int c, sign;
    double frac = 1.0;

    while (isspace(c = getch()));  /* skip white space */
    if (!isdigit(c) && c != EOF && c != '+' && c != '-' && c != '.') {
        ungetch(c);  /* it's not a number */
        return 0;
    }
    sign = (c == '-') ? -1 : 1;
    if (c == '+' || c == '-')
        if (!isdigit(c = getch()) && c != EOF && c != '.') {
            ungetch(c);
            return 0;
        }
    for (*pn = 0.0; isdigit(c); c = getch())
        *pn = 10.0 * *pn + (c - '0');
    if (c == '.') {
        c = getch();
        for (; isdigit(c); c = getch(), frac /= 10)
            *pn = 10.0 * *pn + (c - '0');
    }

    *pn *= sign * frac;
    if (c != EOF)
        ungetch(c);
    return c;
}

int main() {
    int res;
    double n;

    res = getfloat(&n);
    printf("%d %f\n", res, n);
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
