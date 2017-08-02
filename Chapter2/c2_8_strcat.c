#include <stdio.h>


/* Concatenate t to end of s; s must be big enough. */
void strcat_n(char s[], char t[]) {
    int i, j;

    i = j = 0;
    while (s[i] != '\0')  /* find end of s */
        i++;
    while ((s[i++] = t[j++]) != '\0')  /* copy t */
        ;
}


int main(int argc, char *argv[]) {
    char s1[64] = "hello";
    char s2[] = ", world\n";

    strcat_n(s1, s2);
    printf("%s", s1);
}
