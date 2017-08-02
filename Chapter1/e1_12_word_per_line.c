/*
Exercise 1-12.

Write a program that prints its input one word per line.
*/

#include <stdio.h>
#include <stdbool.h>

/* Return true if c is a word character. */
bool iswordchar(char c) {
    return (c >= '0' && c <= '9') ||
           (c >= 'A' && c <= 'Z') ||
           (c >= 'a' && c <= 'z') ||
           (c == '_');
}

int main() {
    int c;

    c = getchar();
    while (c != EOF) {
        if (iswordchar(c)) {
            putchar(c);
            c = getchar();
        } else {
            putchar('\n');
            while (c != EOF && !iswordchar(c))
                c = getchar();
        }
    }
}
