#include <stdio.h>
#include <ctype.h>


/* Convert string s to its double-precision floating-point equivalent. */
double atof_n(const char s[]) {
    double val, power;
    int i, sign;

    for (i = 0; isspace(s[i]); i++)  /* skip white space */
        ;
    sign = (s[i] == '-') ? -1 : 1;

    if (s[i] == '+' || s[i] == '-')
        i++;

    for (val = 0.0; isdigit(s[i]); i++)
        val = 10.0 * val + (s[i] - '0');

    if (s[i] == '.')
        i++;

    for (power = 1.0; isdigit(s[i]); i++) {
        val = 10.0 * val + (s[i] - '0');
        power *= 10.0;
    }

    return sign * val / power;
}


/* Convert string s to integer using atof.*/
int atoi_n(const char s[]) {
    return (int)atof_n(s);
}


int main(int argc, const char *argv[]) {
    if (argc < 2)
        return 0;
    printf("%g\n", atof_n(argv[1]));
}
