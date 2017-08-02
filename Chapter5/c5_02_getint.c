#include <stdio.h>
#include <ctype.h>

/* Get next integer from input into *pn. */
int getint(int *pn) {
    int c, sign;

    while (isspace(c = fgetc(stdin)))   /* skip white space */
        ;
    if (!isdigit(c) && c != EOF && c != '+' && c != '-') {
        ungetc(c, stdin);   /* it's not a number */
        return 0;
    }
    sign = (c == '-') ? -1 : 1;
    if (c == '+' || c == '-')
        c = fgetc(stdin);
    for (*pn = 0; isdigit(c); c = fgetc(stdin))
        *pn = 10 * *pn + (c - '0');
    *pn *= sign;
    if (c != EOF)
        ungetc(c, stdin);
    return c;
}


int main() {
    int n, res;

    res = getint(&n);
    printf("%d %d\n", res, n);
}
