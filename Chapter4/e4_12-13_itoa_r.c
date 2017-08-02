/*
Exercise 4-12.

Adapt the ideas of printd to write a recursive vesion of itoa; that is,
convert an integer into a string by calling a recursive routine.

Exercise 4-13.

Write a recursive version of the function reverse(s), which reverses the
string s in place.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define REVERSE(x)  reverse_r(x, 0, -99)
#define ITOA(s, n, w)  itoa_r(s, n, w, 0)


/* Reverses string s in place. */
void reverse_r(char s[], int l, int r) {
    int temp;

    if (l < r) {
        temp = s[l];
        s[l] = s[r];
        s[r] = temp;
        reverse_r(s, l+1, r-1);
    } else if (r == -99) {
        for(r = 0; s[r]; r++);
        reverse_r(s, 0, r-1);
    }
}


/* Converts n to characters in s, recursively. */
int itoa_r(char s[], int n, int w, int i) {
    if (i == 0 && n == 0) {         /* initial call, n = 0 */
        s[i++] = '0';

    } else if (i == 0 && n) {       /* initial call, n != 0 */
        if (n < 0)
            s[i++] = -(n % 10) + '0';
        else
            s[i++] = (n % 10) + '0';
        i = itoa_r(s, n / 10, w, i);
        if (n < 0)
            s[i++] = '-';

    } else if (n) {                 /* recursive call */
        if (n < 0)
            s[i++] = -(n % 10) + '0';
        else
            s[i++] = (n % 10) + '0';
        return itoa_r(s, n / 10, w, i);
    
    } else                          /* last recursive call */
        return i;

    /* pad with blanks */
    while (i < w)
        s[i++] = ' ';
    s[i] = '\0';
    REVERSE(s);
    return i;
}


int main(int argc, const char *argv[]) {
    char s[BUFSIZ];

    if (argc < 3)
        return 0;
    int n = atoi(argv[1]);
    int w = atoi(argv[2]);
    ITOA(s, n, w);
    printf("%s\n", s);
}
