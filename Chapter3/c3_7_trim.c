#include <stdio.h>
#include <string.h>

/* Remove trailing blanks, tabs, newlines. */
int trim(char s[]) {
    int n;

    for (n = strlen(s) - 1; n >= 0; n--)
        if (s[n] != ' ' && s[n] != '\t' && s[n] != '\n')
            break;
    s[n+1] = '\0';
    return n;
}

int main() {
    char c, s[BUFSIZ] = {};

    for (int i = 0; i < BUFSIZ && (c = getchar()) != EOF; i++)
        s[i] = c;
    trim(s);
    printf("%s", s);
}
