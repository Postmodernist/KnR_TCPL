#include <stdio.h>

/* Return length of string s. */
int strlen_n(char *s) {
    int n;

    for (n = 0; *s != '\0'; s++)
        n++;
    return n;
}

int main() {
    printf("%d\n", strlen_n("hello, world"));
}
