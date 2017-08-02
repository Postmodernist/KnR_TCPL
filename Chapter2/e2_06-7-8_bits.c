/*
Exercise 2-6.

Write a function setbits(x, p, n, y) that returns x with the n bits that begin
at position p set to the rightmost n bits of y, leaving the other bits unchanged.

Exercise 2-7.

Write a function invert(x, p, n) that returns x with the n bits that begin at
position p inverted (i.e., 1 changed into 0 and vice versa), leaving the others
unchanged.

Exercise 2-8.

Write a function rightrot(x, n) that returns the value of the integer x rotated
to the right by n bit positions.
*/

#include <stdio.h>

#define BUF_SIZE 1024
#define DEC(s,t)    { const char *a = (s); t = a[0] == '0' && (a[1] == 'B' || a[1] == 'b') ? \
                    bintoi(a) : atoi_n(a); }
#define ITOBIN(x)   itobin((x), s, BUF_SIZE)

unsigned getbits(unsigned, int, int);
unsigned setbits(unsigned, int, int, unsigned);
unsigned invert(unsigned, int, int);
unsigned rightrot(unsigned, int);
int      bitcount(unsigned);
int      atoi_n(const char*);
unsigned bintoi(const char*);
char     *itobin(unsigned, char*, size_t);


int main(int argc, const char *argv[]) {
    int p, n, mode;
    unsigned x, y;
    char s[BUF_SIZE];

    if ((argc < 2) || (argv[1][0] == 'g' && argc < 5) || (argv[1][0] == 's' && argc < 6) ||
        (argv[1][0] == 'i' && argc < 5) || (argv[1][0] == 'r' && argc < 4) ||
        (argv[1][0] == 'b' && argc < 3)) {
        printf("Usage:\n"
            "\tbits arg0\n\tReturns binary representation of a non-negative integer arg0.\n\n"
            "\tbits MODE arg0 arg1 ...\n"
            "Modes:\n"
            "\tg\tget bits (X P N)    returns N bits of X from position P \n"
            "\ts\tset bits (X P N Y)  returns X with the N bits at P set to N rightmost bits of Y\n"
            "\ti\tinvert (X P N)      returns X with the N bits at P inverted\n"
            "\tr\trotate right (X N)  returns X rotated to the right by N bit positions\n"
            "\tb\tcount bits (X)      returns the number of 1 bits in X.\n");
        return 0;
    }
    
    /* convet argument to binary */
    if (argc == 2) {
        printf("%s\n", ITOBIN(atoi_n(argv[1])));
        return 0;
    }

    mode = argv[1][0];
    DEC(argv[2], x)

    /* mode dispatch */
    switch (mode) {
    
    case 'g':
        p = atoi_n(argv[3]);
        n = atoi_n(argv[4]);
        printf("%s\n", ITOBIN(getbits(x, p, n)));
        break;
    
    case 's':
        p = atoi_n(argv[3]);
        n = atoi_n(argv[4]);
        DEC(argv[5], y)
        printf("%s\n", ITOBIN(setbits(x, p, n, y)));
        break;
    
    case 'i':
        p = atoi_n(argv[3]);
        n = atoi_n(argv[4]);
        printf("%s\n", ITOBIN(invert(x, p, n)));
        break;

    case 'r':
        n = atoi_n(argv[3]);
        printf("%s\n", ITOBIN(rightrot(x, n)));
        break;

    case 'b':
        printf("%d\n", bitcount(x));
        break;

    default:
        fprintf(stderr, "Invalid mode: %c\n", mode);
        break;
    }    
    return 0;
}


/* ------------------------------------------------------------------------- */
/* Bitwise methods */


/* Get N bits from position P. */
unsigned getbits(unsigned x, int p, int n) {
    return (x >> (p + 1 - n)) & ~(~0 << n);
}


/* Returns X with the N bits at P set to N rightmost bits of Y.*/
unsigned setbits(unsigned x, int p, int n, unsigned y) {
    x = x & ~(~(~0 << n) << (p + 1 - n));
    y = (y & ~(~0 << n)) << (p + 1 - n);
    return x | y;
}


/* Returns X with the N bits at P inverted.*/
unsigned invert(unsigned x, int p, int n) {
    return x ^ (~(~0 << n) << (p + 1 - n));
}


/* Returns X rotated to the right by N bit positions.*/
unsigned rightrot(unsigned x, int n) {
    int i;
    unsigned tmp;
    
    for (i = 0, tmp = x; tmp; i++, tmp >>= 1);
    n %= i;
    return (x >> n | x << (i - n)) & ~(~0 << i);
}


/* Returns the number of 1 bits in X. */
int bitcount(unsigned x) {
    int b;

    for (b = 0; x; x >>= 1)
        if (x & 1)
            b++;
    return b;
}


/* ------------------------------------------------------------------------- */
/* Utilities */


/* Convert decimal string s to integer.*/
int atoi_n(const char s[]) {
    int i, n;

    n = 0;
    for (i = 0; s[i] >= '0' && s[i] <= '9'; ++i)
        n = 10 * n + (s[i] - '0');
    return n;
}


/* Convert string to unsigned integer, assuming binary form. */
unsigned bintoi(const char s[]) {
    unsigned i, n;

    n = 0;
    i = 2;
    for (; s[i] == '0' || s[i] == '1'; i++) {
        n <<= 1;
        if (s[i] == '1')
            n++;
    }
    return n;
}


/* Convert integer to binary string and leave it in s. */
char *itobin(unsigned n, char s[], size_t lim) {
    int i, j, c;

    /* convert to reversed string first */
    for (i = 0; n && i < lim; i++) {
        if (n & 1)
            s[i] = '1';
        else
            s[i] = '0';
        n >>= 1;
    }
    if (i == 0)
        s[i++] = '0';

    /* assert s has enough space */
    if (i + 3 >= lim) {
        s[0] = '\0';
        return s;
    }

    s[i++] = 'b';
    s[i]   = '0';
    s[i+1] = '\0';

    /* reverse s */
    for (j = 0; j < i; j++, i--) {
        c = s[j];
        s[j] = s[i];
        s[i] = c;
    }
    return s;
}
