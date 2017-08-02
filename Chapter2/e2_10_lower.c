/*
Exercise 2-10.

Rewrite the function lower, which converts upper case letters to lower case,
with a conditional expression instead of if-else.
*/

#include <stdio.h>

/* Convert c to lower case; ASCII only. */
int lower_n(int c) {
    return (c >= 'A' && c <= 'Z') ? c + 'a' - 'A' : c;
}

int main() {
    int i;
    char s[] = "HeLLo, WoRLD";

    for (i = 0; s[i]; i++) 
        putchar(lower_n(s[i]));
    putchar('\n');
}
