#include <stdio.h>

/* Return length of s. */
int strlen_n(const char s[]) {
    int i;
    
    i = 0;
    while (s[i] != '\0')
        ++i;
    
    return i;
}

int main() {
    printf("%d\n", strlen_n("hello, world"));
}
