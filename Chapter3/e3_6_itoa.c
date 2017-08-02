/*
Exercise 3-6.

Write a version of itoa that accepts three arguments instead of two. The third
argument is a minimum field width; the converted number must be padded with
blanks on the left if necessary to make it wide enough.
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
char *itoa(int n, char s[], int w) {
    int i, k;
    
    i = 0, k = n;
    if (n < 0) {
        do {
            s[i++] = -(k % 10) + '0';
        } while ((k /= 10) != 0);
        s[i++] = '-';
    } else
        do {
            s[i++] = k % 10 + '0';
        } while ((k /= 10) != 0);
    /* pad with blanks */
    while (i < w)
        s[i++] = ' ';
    s[i] = '\0';
    reverse(s);
    return s;
}


int main(int argc, const char* argv[]) {
    char s[BUFSIZ];

    if (argc < 3) {
        printf("Usage:  itoa NUMBER WIDTH\n");
        return 0;
    }

    int n = atoi(argv[1]);
    int w = atoi(argv[2]);
    printf("%s\n", itoa(n, s, w));
}
