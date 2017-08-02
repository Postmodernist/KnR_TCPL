/* Reverse Polish calculator. */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>


/* ---------------------------------------------------------------------- */
/* Main */

enum { INV, NUM, SUM, SUB, MUL, DIV, MOD };
static const char* typestr[] = {"invalid operator", "number", "+", "-", "*", "/", "%"};
extern double stack[];
extern double* sp;

int    gettoken(char*, double*);
void   stack_push(double);
double stack_pop(void);
int    stack_len(void);

int main(int argc, char** argv) {
    if (argc == 1) {
        printf("\x1b[93m" "Expression Calculator\n" "\x1b[0m"
               "Usage: expr EXPRESSION\n"
               "Input expression in reverse Polish notation (e.g. 4 1 + 2 *).\n"
               "Supported operators: + - * / %%.\n");
        return 0;
    }

    int type;
    double tok, op1, op2;

    for (char** arg = argv + 1; *arg; arg++) {
        type = gettoken(*arg, &tok);

        if (type == NUM) {
            stack_push(tok);
            continue;
        } else if (type == INV) {
            printf("Error: unknown operator '%s'.\n", *arg);
            exit(EXIT_FAILURE);
        }

        if (stack_len() > 1) {
            op2 = stack_pop();
            op1 = stack_pop();
        } else {
            printf("Error: not enough operands for '%s'.\n", typestr[type]);
            exit(EXIT_FAILURE);
        }

        switch(type) {

        case SUM:
            if (isinf(op1) && isinf(op2))
                stack_push(NAN);
            else
                stack_push(op1 + op2);
            break;

        case SUB:
            if (isinf(op1) && isinf(op2))
                stack_push(NAN);
            else if ((isinf(op1) && !isnan(op2))  ||  (!isnan(op1) && isinf(op2)))
                stack_push(INFINITY);
            else
                stack_push(op1 - op2);
            break;

        case MUL:
            if ((!op1 && isinf(op2))  || (isinf(op1) && !op2))
                stack_push(NAN);
            else
                stack_push(op1 * op2);
            break;

        case DIV:
            if (op2)
                stack_push(op1 / op2);
            else
                stack_push((op1 && !isnan(op1)) ? INFINITY : NAN);
            break;

        case MOD:
            if (isnormal(op1) && isnormal(op2))
                stack_push(fmod(op1, op2));
            else
                stack_push(0.0);
            break;

        default:
            printf("Error: something went wrong with this -- '%s'.\n", *arg);
            exit(EXIT_FAILURE);
            break;
        }
    }

    // Print result
    if (stack_len() == 1) {
        printf("\x1b[91m" "ans =\t%.8g\n" "\x1b[0m", stack_pop());
    
    } else if (stack_len() > 1) {
        printf("\x1b[91m" "ans =\t%.8g\n" "\x1b[0m", stack_pop());
        printf("Remaining stack: ");
        for (double* dp = stack; dp <= sp; dp++)
            printf("%.8g ", *dp);
        putchar('\n');
    }

    return 0;
}

/* Get token from string argument. */
int gettoken(char* s, double* tok) {

    if (isdigit(s[0]) ||                                                     // 5
        (strlen(s) > 1 && (s[0] == '-' || s[0] == '.') && isdigit(s[1])) ||  // -5 || .5
        (strlen(s) > 2 &&  s[0] == '-' && s[1] == '.'  && isdigit(s[2]))) {  // -.5
        *tok = atof(s);
        return NUM;                     // number

    } else if (strlen(s) != 1) {
        return INV;                     // invalid token

    } else if (s[0] == '+') {
        return SUM;                     // sum operator

    } else if (s[0] == '-') {
        return SUB;                     // subtraction operator

    } else if (s[0] == '*') {
        return MUL;                     // multiplication operator

    } else if (s[0] == '/') {
        return DIV;                     // division operator

    } else if (s[0] == '%') {
        return MOD;                     // modulus operator
    }

    return INV;
}

/* ---------------------------------------------------------------------- */
/* Stack */

#define MAX_STACK_LEN 100   // maximum depth of val stack

double stack[MAX_STACK_LEN];
double* sp = stack - 1;

/* Push val onto the stack. */
void stack_push(double val) {
    if (stack_len() < MAX_STACK_LEN)
        *++sp = val;
    else
        printf("Error: stack is full, can't push %g.\n", val);
}

/* Pop and return top value from stack. */
double stack_pop(void) {
    if (stack_len())
        return *sp--;
    else {
        printf("Error: stack is empty, returning NaN.\n");
        return NAN;
    }
}

/* Returns the size of the stack. */
int stack_len() {
    return sp + 1 - stack;
}

