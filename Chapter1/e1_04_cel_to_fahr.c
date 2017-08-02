/*
Exercise 1-4.

Write a program to print the corresponding Celsius to Fahrenheit table.
*/

#include <stdio.h>

int main() {
    float fahr, celsius;
    int lower, upper, step;

    lower = -50;    /* lower limit of temperature table */
    upper = 70;     /* upper limit */
    step  = 10;     /* step size */

    printf("   C      F\n"
           "-----------\n");

    celsius = lower;
    while (celsius <= upper) {
        fahr = celsius * (9.0 / 5.0) + 32.0;
        printf("%4.0f %6.1f\n", celsius, fahr);
        celsius += step;
    }
}
