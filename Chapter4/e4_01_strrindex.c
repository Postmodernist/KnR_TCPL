/*
Exercise 4-1.

Write the function strrindex(s, t), which returns the position of the rightmost
occurence of t in s, or -1 if there is none.
*/

#include <stdio.h>

#define LEN(s,i)    for (i = 0; s[i]; i++)

/* Return index of rightmost occurence of t in s, -1 if none. */
int strrindex(char s[], char t[]) {
    int i, j, k, slen, tlen;

    LEN(s, slen);
    LEN(t, tlen);
    if (!slen || !tlen)
        return -1;
    for (i = slen-1; i >= 0; i--) {
        for (j = i, k = tlen-1; k >= 0 && j >= 0 && s[j] == t[k]; j--, k--)
            ;
        if (k < 0)
            return j+1;
    }
    return -1;
}


int main() {
    char s[] = "Would not we shatter it to bits - and then";
    printf("%d\n", strrindex(s, "it"));
}
