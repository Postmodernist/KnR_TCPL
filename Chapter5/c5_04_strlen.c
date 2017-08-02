#include <stdio.h>

/* Return length of string s. */
int strlen_n(char *s) {
    char *p = s;

    while (*p != '\0')
        p++;
    return p - s;
}

int main() {
    printf("%d\n", strlen_n("hello, world"));
}
