/*
Exercise 2-4.

Write an alternate version of squeeze(s1, s2) that deletes each character in
s1 that matches any character in the string s2.
*/

#include <stdio.h>
#include <stdbool.h>


/* Return true if str contains chr. */
bool contains(char *str, char chr) {
    for (; *str; str++)
        if (*str == chr)
            return true;
    return false;
}


/* Delete each character in s1 that matches any character in s2*/
void squeeze(char *s1, char *s2) {
    int i, j;

    for (i = j = 0; s1[i] != '\0'; i++)
        if (!contains(s2, s1[i]))
            s1[j++] = s1[i];
    s1[j] = '\0';
}


int main() {
    char s[] = "hello, world";
    squeeze(s, "oe");
    printf("%s\n", s);
    return 0;
}
