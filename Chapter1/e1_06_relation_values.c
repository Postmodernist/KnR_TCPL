/*
Exercise 1-6.

Verify that the expression getchar() != EOF is 0 or 1.
*/

#include <stdio.h>

int main() {
    int c, res;

    while (1) {
        res = ((c = getchar()) != EOF);
        printf("%d ", res);
    }
}
