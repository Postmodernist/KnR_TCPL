/*
Exercise 3-4.

In a two's complement number representation, our version of itoa does not
handle the largest negative number, that is, the value of n equal to
-(2^(wordsize-1)). Explain why not. Modify it to print that value correctly,
regardless of the machine on which it runs.
*/

#include <stdio.h>
#include <string.h>
#include <limits.h>


/* Reverse string s in place. */
void reverse(char s[]) {
    int c, i, j;
    for (i = 0, j = strlen(s) - 1; i < j; i++, j--)
        c = s[i], s[i] = s[j], s[j] = c;
}


/* Convert n to characters in s. */
void itoa(int n, char s[]) {
    int i;

    i = 0;
    if (n < 0) {
        do {  
            s[i++] = -(n % 10) + '0';
        } while ((n /= 10) != 0);
        s[i++] = '-';
    } else
        do {
            s[i++] = n % 10 + '0';
        } while ((n /= 10) > 0);
    s[i] = '\0';
    reverse(s);
}


int main(void) {
    char s[BUFSIZ] = {};

    itoa(INT_MIN, s);
    printf("%s\n", s);
}
