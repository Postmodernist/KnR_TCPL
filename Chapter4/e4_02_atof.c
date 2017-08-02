/*
Exercise 4-2.

Extend atof to handle scientific notation of the form

    123.45e-6

where a floating-point number may be followed by e or E and an optionally
signed exponent.
*/

#include <ctype.h>
#include <float.h>
#include <stdio.h>

#define NUM(x)  ((x) - '0')

/* Convert string s to its double-precision floating-point equivalent. */
double atof_n(const char *ps) {
    double sign, val, power;

    for (; isspace(*ps); ps++);  /* skip white space */

    sign = (*ps == '-') ? -1.0 : 1.0;
    val = 0.0;
    power = 1.0;

    if (*ps == '+' || *ps == '-')
        ps++;

    for (; isdigit(*ps); ps++)
        val = 10.0 * val + NUM(*ps);

    if (*ps == '.') {
        ps++;
        for (; isdigit(*ps); ps++) {
            val = 10.0 * val + NUM(*ps);
            power *= 10.0;
        }
    }

    if ((*ps == 'e' || *ps == 'E') && (*++ps == '-' || *ps == '+' || isdigit(*ps))) {
        int esign, exp;

        esign = (*ps == '-') ? -1 : 1;
        
        if (!isdigit(*ps))
            ps++;

        for(exp = 0; isdigit(*ps); ps++)
            exp = exp * 10 + NUM(*ps);

        if (esign == 1)
            for (; exp > 0; exp--)
                power /= 10.0;
        else
            for (; exp > 0; exp--)
                power *= 10.0;
    }

    return sign * val / power;
}


int main(int argc, const char *argv[]) {
    double a, atmp;
    int d, f;

    if (argc < 2)
        return 0;

    a = atof_n(argv[1]);  /* number to display */

    /* do best to display only reliable part of the number, which is DBL_DIG long */
    atmp = a < 0 ? -a : a;
    for (d = 0; atmp >= 1.0; atmp /= 10.0, d++);
    
    f = DBL_DIG - d;
    f = f > 0 ? f : 1;  /* decimal part */
    d = d > 0 ? d : 1;  /* integer part */

    printf("%*.*f\n", d, f, a);

    return 0;
}


