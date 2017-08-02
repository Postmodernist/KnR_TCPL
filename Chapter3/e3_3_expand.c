/*
Exercise 3-3.

Write a function expand(s1, s2) that expands shorthand notations like a-z in
the string s1 into the equivalent complete list abc...xyz in s2. Allow for
letters of either case and digits, and be prepared to handle cases like a-b-c
and a-z0-9 and -a-z. Arrange that a leading or trailing - is taken literally.
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>


/* Expand shorthand notations like a-z in the string s1 into the equivalent complete
list abc...xyz in s2. */
void expand(char s1[], char s2[]) {
    int i, j, k, step;

    for (i = 0, j = 0; i < strlen(s1) - 1; i++) {
        s2[j++] = s1[i];  /* copy current character */

        if (s1[i+1] == '-' &&
            ((isupper(s1[i]) && isupper(s1[i+2])) ||
             (islower(s1[i]) && islower(s1[i+2])) ||
             (isdigit(s1[i]) && isdigit(s1[i+2])))) {
            step = (s1[i] > s1[i+2]) ? -1 : 1;
            for (k = s1[i] + step; k != s1[i+2]; k += step)
                s2[j++] = k;  /* expand the shorthand sequence */
            i++;  /* skip the '-' */
        }
    }

    s2[j] = s1[i];  /* copy the last character */
}


int main(int argc, char* argv[]) {
    char s[1024] = {};

    if (argc < 2)
        return 0;

    expand(argv[1], s);
    printf("%s\n", s);
}
