/* ---------------------------------------------------------------------- */
/* Stack */

#include <stdio.h>
#include <math.h>
#include <rpcalc.h>

#define MAXVAL      100     // maximum depth of val stack

int sp = 0;                 // next free stack position
double val[MAXVAL];         // value stack


/* Push f onto value stack. */
void push(double f) {
    // printf("Pushing %g\n", f);
    if (sp < MAXVAL)
        val[sp++] = f;
    else
        printf("Stack is full, can't push %g.\n", f);
}


/* Pop and return top value from stack. */
double pop(void) {
    // printf("Popping\n");
    if (sp)
        return val[--sp];
    else {
        printf("Stack is empty, returning NaN.\n");
        return NAN;
    }
}


/* Return top element on the stack w/o popping it. */
double peek() {
    if (sp)
        return val[sp-1];
    else
        return NAN;
}


/* Print stack contents. */
void view() {
    printf("stack =\t[");
    for (int i = 0; i < sp; i++)
        if (i + 1 < sp)
            printf("%g, ", val[i]);
        else
            printf("%g", val[i]);
    printf("]\n");
}


/* Flush the stack. */
void flush() {
    sp = 0;
}


/* Return the size of the stack. */
int stacklen() {
    return sp;
}

