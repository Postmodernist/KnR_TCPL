/*
Exercise 7-4.

Write a private version of scanf analogous to minprintf from the previous
section.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

#define MAXLINE     1000
#define MAXWORD     100

int getword(char *, int);
int minscanf(char *, ...);

int main() {
    int a;
    double b;
    char s[100];

    printf("int\n");
    minscanf("%d", &a);
    printf("%d\n", a);

    printf("abc int: str/double\n");
    minscanf("abc %d: %s/%f", &a, &s, &b);
    printf("%d, %s, %f\n", a, s, b);
}

int minscanf(char *fmt, ...) {
    va_list ap;
    char    *p, *p0, buf[MAXWORD], ibuf[MAXWORD], tmp[MAXWORD], *cp;
    int     wlen, nscans, err, *ip;
    double  *dp;

    va_start(ap, fmt);  /* make ap point to 1st unnamed arg */
    err = nscans = 0;
    p = fmt;
    while (1) {
        /* skip white space */
        for (; isspace(*p); p++);
        /* break if fmt is consumed */
        if (*p == '\0')
            break;
        /* read format word and compare to input word */
        if (*p != '%') {
            /* search end of word */
            for (p0 = p; isalnum(*p) || *p == '_'; p++);
            if (!(wlen = p - p0))
                wlen++, p++;
            /* read word */
            if (wlen < MAXWORD) {
                strncpy(buf, p0, wlen);
                buf[wlen] = '\0';
            } else
                printf("error: minscanf: not enough space in buffer\n");
            /* read input word and compare */
            if (getword(ibuf, MAXWORD) == EOF || strcmp(buf, ibuf) != 0) {
                printf("error: minscanf: invalid input\n");
                break;
            }
        /* read format specifier */
        } else {
            switch (*++p) {
            case 'd':
                ip = va_arg(ap, int*);
                if (getword(ibuf, MAXWORD) != EOF) {
                    if (isdigit(ibuf[0])) {
                        *ip = atoi(ibuf);
                        nscans++;
                    } else {
                        printf("error: minscanf: expected a number\n");
                        err = 1;
                    }
                } else {
                    printf("error: minscanf: unexpected end of input\n");
                    err = 1;
                }
                break;
            case 'f':
                dp = va_arg(ap, double*);
                if (getword(ibuf, MAXWORD) != EOF) {
                    if (isdigit(ibuf[0])) {
                        if (getword(tmp, MAXWORD) != EOF && tmp[0] == '.') {
                            strcat(ibuf, ".");
                            if (getword(tmp, MAXWORD) != EOF && isdigit(tmp[0]))
                                strcat(ibuf, tmp);
                        }
                        *dp = atof(ibuf);
                        nscans++;
                    } else {
                        printf("error: minscanf: expected a number\n");
                        err = 1;
                    }
                } else {
                    printf("error: minscanf: unexpected end of input\n");
                    err = 1;
                }
                break;
            case 's':
                cp = va_arg(ap, char*);
                if (getword(ibuf, MAXWORD) != EOF) {
                    strcpy(cp, ibuf);
                    nscans++;
                } else {
                    printf("error: minscanf: unexpected end of input\n");
                    err = 1;
                }
                break;
            case '%':
                if (getword(ibuf, MAXWORD) == EOF || strcmp("%", ibuf) != 0) {
                    printf("error: minscanf: invalid input\n");
                    err = 1;
                }
                break;
            default:
                printf("error: minscanf: unrecognized format specifier '%c'\n", *p);
                err = 1;
                break;
            }
            if (err)
                break;
            p++;
        }
    }
    va_end(ap);
    return nscans;
}

/* Get word from stdin */
int getword(char *s, int lim) {
    static char buf[MAXLINE + 1] = {};
    static char *pbuf, *pend;
    int wlen;

    s[0] = '\0';
    
    while (1) {
        /* read input string */
        if (!strlen(buf))
            if ((pbuf = fgets(buf, MAXLINE, stdin)) == NULL)
                return EOF;
        if (strlen(buf) == MAXLINE)
            printf("warning: getword: line buffer is full\n");
        /* search start of word */
        for (; isspace(*pbuf); pbuf++);
        /* keep reading until word or EOF is found */
        if (*pbuf == '\0')
            buf[0] = '\0';
        else
            break;
    }
    
    /* search end of word */
    for (pend = pbuf; isalnum(*pend) || *pend == '_'; pend++);
    if (!(wlen = pend - pbuf))
        wlen++, pend++;

    /* get word */
    if (wlen < lim) {
        strncpy(s, pbuf, wlen);
        s[wlen] = '\0';
    } else
        printf("error: getword: not enough space in word buffer\n");
    
    /* reset buf if it's consumed */
    if (*(pbuf = pend) == '\0')
        buf[0] = '\0';
    
    return s[0];
}
