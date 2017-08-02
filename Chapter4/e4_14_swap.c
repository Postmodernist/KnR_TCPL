/*
Exercise 4-14.

Define a macro swap(t, x, y) that interchanges two arguments of type t.
(Block structure will help.)
*/

#include <stdio.h>

#define swap(t, x, y)    { t tmp = x; x = y; y = tmp; }

int main() {
    double k = 3.0, l = 0.4;
    printf("%g %g\n", k, l);
    swap(double, k, l);
    printf("%g %g\n", k, l);
}
