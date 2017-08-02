/* ---------------------------------------------------------------------- */
/* Variables */

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <rpcalc.h>

#define VARNUM 27

double var[VARNUM];  // variables array: a..z, ans


/* Initialize variables array. */
void initvar() {
    for (double* pvar = var; pvar < var + VARNUM; pvar++)
        *pvar = NAN;
}


/* Get variable value. */
double getvar(char v) {
    if (islower(v) || v == '{')
        return var[v-'a'];
    else {
        printf("Invalid variable name '%c'.\n", v);
        return NAN;
    }
}


/* Set variable value. */
void setvar(char v, double x) {
    if (islower(v) || v == '{')
        var[v-'a'] = x;
    else
        printf("Invalid variable name '%c'.\n", v);
}


/* Prints out assigned variables. */
void vlist() {
    double* pvar;
    int vn;
    
    for (pvar = var, vn = 'a'; pvar < var + VARNUM - 1; pvar++, vn++)
        if (!isnan(*pvar))
            printf("%c =\t%g\n", vn, *pvar);
    if (!isnan(*pvar))
        printf("ans =\t%g\n", *pvar);
}

