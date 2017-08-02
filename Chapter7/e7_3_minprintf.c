/*
Exercise 7-3.

Revise minprintf to handle more of the other facilities of printf.
*/

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Minimal printf with variable argument list */
void minprintf(char *fmt, ...) {
    va_list ap;  /* points to each unnamed arg in turn */
    char    *p, *p0, *sval, buf[20];
    int     ival, width, precision, align;
    double  dval;
    void    *pval;

    va_start(ap, fmt);  /* make ap point to 1st unnamed arg */
    width = precision = align = 0;
    for (p = fmt; *p; p++) {
        if (*p != '%') {
            putchar(*p);
            continue;
        }
        p++;
        /* read parameters */
        if (*p == '-') {
            align = 1;
            p++;
        }
        if (isdigit(*p)) {
            for (p0 = p; isdigit(*p); p++);
            strncpy(buf, p0, p - p0);
            buf[p - p0] = '\0';
            width = atoi(buf);
        }
        if (*p == '.') {
            p++;
            if (isdigit(*p)) {
                for (p0 = p; isdigit(*p); p++);
                strncpy(buf, p0, p - p0);
                buf[p - p0] = '\0';
                precision = atoi(buf);
            }
        }
        /* read format specifier */
        switch (*p) {
        case 'd':
            ival = va_arg(ap, int);
            printf(align ? "%-*.*d" : "%*.*d", width, precision, ival);
            break;
        case 'f':
            dval = va_arg(ap, double);
            precision = precision ? precision : 6;
            printf(align ? "%-*.*f" : "%*.*f", width, precision, dval);
            break;
        case 's':
            sval = va_arg(ap, char*);
            precision = precision ? precision : strlen(sval);
            printf(align ? "%-*.*s" : "%*.*s", width, precision, sval);
            break;
        case 'p':
            pval = va_arg(ap, void*);
            printf(align ? "%-*p" : "%*p", width, pval);
            break;
        case '%':
            putchar('%');
            break;
        default:
            putchar(*p);
            break;
        }
    }
    va_end(ap);  /* clean up when done */
}

int main() {
    char ps[] = "string";
    int i = 1;

    minprintf("%2d) '%-10.3s'\n", i++, ps);
    minprintf("%3d) %10.4f\n", i++, 3.141592);
    minprintf("%4.3d) %20p\n", i++, ps);
    minprintf("%.4d) %%\n", i++);
}
