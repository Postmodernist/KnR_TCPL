/*
Exercise 3-5.

Write the function itob(n, s, b) that converts the integer n into a base b
character representation in the string s. In particular, itob(n, s, 16)
formats n as a hexadecimal integer in s.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* Reverse string s in place. */
void reverse(char s[]) {
    int c, i, j;
    for (i = 0, j = strlen(s) - 1; i < j; i++, j--)
        c = s[i], s[i] = s[j], s[j] = c;
}


/* Convert n to characters in s. */
char *itob(int n, char s[], int b) {
    const char digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    int i, k, sign;

    if (b > strlen(digits)) {
        printf("Maximum base is %lu\n", strlen(digits));
        return NULL;
    } else if (b < 2) {
        printf("Minimum base is 2\n");
        return NULL;
    }

    sign = n < 0 ? -1 : 1;
    i = 0, k = n;
    do {
        s[i++] = digits[sign * (k % b)];
    } while ((k /= b) != 0);
    if (n < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
    return s;
}


int main(int argc, char* argv[]) {
    char s[BUFSIZ];

    if (argc < 3)
        return 0;

    int n = atoi(argv[1]);
    int b = atoi(argv[2]);
    if (itob(n, s, b))
        printf("%s\n", s);
    return 0;
}
