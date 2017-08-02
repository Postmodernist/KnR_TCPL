/*
Exercise 2-5.

Write the function any(s1, s2), which returns the first location in the string
s1 where any character from the string s2 occurs, or -1 if s1 contains no
characters from s2. (The standard library function strpbrk does the same job
but returns a pointer to the location.)
*/

#include <stdio.h>
#include <stdbool.h>


/* Return true if str contains chr. */
bool contains(const char *str, char chr) {
    for (; *str; str++)
        if (*str == chr)
            return true;
    return false;
}


/* Return the first location in s1 where any character from s2 occurs, or -1. */
size_t any(const char *s1, const char *s2) {
    size_t i;

    for (i = 0; s1[i] != '\0'; i++)
        if (contains(s2, s1[i]))
            return i;
    return -1;
}


int main() {
    char s[] = "hello, world";
    
    printf("%lu\n", any(s, "od"));
    return 0;
}
