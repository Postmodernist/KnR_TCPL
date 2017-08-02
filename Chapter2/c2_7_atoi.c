ить#include <stdio.h>

/* Convert s to integer; 1st version.*/
int atoi_n(char s[]) {
    int i, n;

    n = 0;
    for (i = 0; s[i] >= '0' && s[i] <= '9'; ++i)
        n = 10 * n + (s[i] - '0');

    return n;
}

int main() {
    printf("%d\n", atoi_n("123"));
}
