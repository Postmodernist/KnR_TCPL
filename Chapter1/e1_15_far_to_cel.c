/*
Exercise 1-15.

Rewrite the temperature conversion program of Section 1.2 to use a function for
conversion.
*/

#include <stdio.h>

#define LOWER 0         /* lower limit of table */
#define UPPER 300       /* upper limit */
#define STEP  20        /* step size */


/* Convert temperature from Fahrenheit to Celsius. */
double fahrtocel(double t) {
    return (5.0 / 9.0) * (t - 32.0);
}


/* Print Fahrenheit-Celsius table. */
int main() {
    float fahr;

    printf("  F      C\n"
           "----------\n");
    for (int fahr = LOWER; fahr <= UPPER; fahr = fahr + STEP)
        printf("%3d %6.1f\n", fahr, fahrtocel(fahr));
}
