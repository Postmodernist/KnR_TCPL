/*
Exercise 2-3.

Write the function htoi(s), which converts a string of hexadecimal digits
(including an optional 0x or 0X) into its equivalent integer value. The
allowable digits are 0 through 9, a through f, and A through F.
*/

#include <stdio.h>

#define ISDIGIT(x) (((x) >= '0' && (x) <= '9') || \
                    ((x) >= 'A' && (x) <= 'F') || \
                    ((x) >= 'a' && (x) <= 'f'))

#define DEC(x)     ((x) >= '0' && (x) <= '9' ? (x) - '0' : \
                    (x) >= 'A' && (x) <= 'Z' ? (x) - '7' : (x) - 'W')


/* Convert hexadecimal string to its integer equivalent. */
long int htoi(char s[]) {
    long int d; 
    int i, sign;

    if (s[0] == '\0')  /* zero-length string */
        return 0;

    if (s[0] == '+' || s[0] == '-') {
        sign = s[0] == '+' ? 1 : -1;
        i = s[1] == '0' && (s[2] == 'X' || s[2] == 'x') ? 3 : 1;
    } else {
        sign = 1;
        i = s[0] == '0' && (s[1] == 'X' || s[1] == 'x') ? 2 : 0;
    }
    
    d = 0;
    for (; ISDIGIT(s[i]); i++)
        d = d * 16 + DEC(s[i]) * sign;
    return d;
}


int main(void) {
    printf("%ld\n", htoi("A"));             /* 10 */
    printf("%ld\n", htoi("b"));             /* 11 */
    printf("%ld\n", htoi("-12AB"));         /* -4779 */
    printf("%ld\n", htoi("0Xfeed"));        /* 65261 */
    printf("%ld\n", htoi("0xDEADBEEF"));    /* 3735928559 */
    printf("%ld\n", htoi("-0x8000000000000000"));  /* -9223372036854775808 */
}
