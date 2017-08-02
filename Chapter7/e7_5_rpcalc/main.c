/* ---------------------------------------------------------------------- */
/* Reverse Polish Expression Calculator. */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <rpcalc.h>


int main() {
    int type, echo = 0;
    double op1, op2, anstmp = NAN;
    char s[MAXTOKEN];
    char cvar = '\0';

    printf("\x1b[93m" "Expression Calculator\n" "\x1b[0m"
           "Input expressions in reverse Polish notation (e.g. 4 1 + 2 *).\n"
           "Press Enter to evaluate or Ctrl-D to exit.\n"
           "Supported operators: + - * / %% sin exp pow.\n"
           "Valid variable names: a, b ... z.\n"
           "Type 'help' for commands info.\n");

    initinput();
    initvar();
    printf(">> ");
    
    while ((type = gettoken(s)) != EOF) {
    
        if (cvar && type != SET && type != VPRINT) {
            if (!isnan(op1 = getvar(cvar)))
                push(op1), echo = 1;  // push variable value to stack
            else if (cvar == '{')
                printf("Variable 'ans' is undefined.\n"), echo = 0;
            else
                printf("Variable '%c' is undefined.\n", cvar), echo = 0;
            cvar = '\0';
        }

        switch (type) {

        case NUM:
            sscanf(s, "%lf", &op1);
            push(op1);
            echo = 0;
            break;

        case SUM:
            if (stacklen() > 1) {
                op2 = pop(), op1 = pop();
                if (isinf(op1) && isinf(op2))
                    push(NAN);
                else
                    push(op1 + op2);
                echo = 1;
            } else
                printf("Not enough operands for '+'.\n"), echo = 0;
            break;

        case SUB:
            if (stacklen() > 1) {
                op2 = pop(), op1 = pop();
                if (isinf(op1) && isinf(op2))
                    push(NAN);
                else if ((isinf(op1) && !isnan(op2))  ||  (!isnan(op1) && isinf(op2)))
                    push(INFINITY);
                else
                    push(op1 - op2);
                echo = 1;
            } else
                printf("Not enough operands for '-'.\n"), echo = 0;
            break;

        case MUL:
            if (stacklen() > 1) {
                op2 = pop(), op1 = pop();
                if ((!op1 && isinf(op2))  || (isinf(op1) && !op2))
                    push(NAN);
                else
                    push(op1 * op2);
                echo = 1;
            } else
                printf("Not enough operands for '*'.\n"), echo = 0;
            break;

        case DIV:
            if (stacklen() > 1) {
                op2 = pop(), op1 = pop();
                if (op2)
                    push(op1 / op2);
                else
                    push((op1 && !isnan(op1)) ? INFINITY : NAN);
                echo = 1;
            } else
                printf("Not enough operands for '/'.\n"), echo = 0;
            break;

        case MOD:
            if (stacklen() > 1) {
                op2 = pop(), op1 = pop();
                if (isnormal(op1) && isnormal(op2))
                    push((double)((int)op1 % (int)op2));
                else
                    push(0.0);
                echo = 1;
            } else
                printf("Not enough operands for '%%'.\n"), echo = 0;
            break;

        case SIN:
            if (stacklen())
                push(sin(pop())), echo = 1;
            else
                printf("No operand for 'sin'.\n"), echo = 0;
            break;

        case EXP:
            if (stacklen())
                push(exp(pop())), echo = 1;
            else
                printf("No operand for 'exp'.\n"), echo = 0;
            break;

        case POW:
            if (stacklen() > 1) {
                op2 = pop(), op1 = pop();
                push(pow(op1, op2)), echo = 1;
            } else
                printf("Not enough operands for 'pow'.\n"), echo = 0;
            break;

        case STACK:          // view stack contents
            view();
            echo = 0;
            break;

        case DUP:           // duplicate last element
            if (stacklen()) {
                op1 = pop();
                push(op1), push(op1);
                printf("[%g] -> [%g, %g]\n", op1, op1, op1);
            } else
                printf("Must be at least one number to duplicate.\n");
            echo = 0;
            break;

        case SWP:           // swap top two elements
            if (stacklen() > 1) {
                op2 = pop(), op1 = pop();
                push(op2), push(op1);
                printf("[%g, %g] -> [%g, %g]\n", op1, op2, op2, op1);
            } else
                printf("Must be at least two numbers to swap.\n");
            echo = 0;
            break;

        case CLR:           // clear stack
            flush();
            printf("Stack has been flushed.\n");
            echo = 0;
            break;

        case POP:           // pop elements from the stack
            if (stacklen()) {
                for (int i = (int)pop(); i && stacklen(); pop(), i--);
                view();
            } else
                printf("No operand for 'pop'.\n");
            echo = 0;
            break;

        case VAR:           // prime variable handling
            cvar = s[0];
            echo = 0;
            break;

        case VPRINT:        // print variable value
            if (cvar) {
                if (!isnan(op1 = getvar(cvar))) {
                    if (cvar == '{')
                        printf("ans =\t%g\n", op1);
                    else
                        printf("%c =\t%g\n", cvar, op1);
                } else if (cvar == '{')
                    printf("Variable 'ans' is undefined.\n");
                else
                    printf("Variable '%c' is undefined.\n", cvar);
            } else
                printf("No variable name is given.\n");
            cvar = '\0';
            echo = 0;
            break;

        case SET:           // assign value to variable
            if (cvar) {
                if (stacklen()) {
                    op1 = pop();
                    setvar(cvar, op1);
                    if (cvar == '{')
                        printf("ans =\t%g\n", op1);
                    else
                        printf("%c =\t%g\n", cvar, op1);
                } else if (cvar == '{')
                    printf("No value to assign to variable 'ans' is given.\n");
                else
                    printf("No value to assign to variable '%c' is given.\n", cvar);
            } else
                printf("No variable name is given.\n");
            cvar = '\0';
            echo = 0;
            break;

        case VLIST:         // print variable value
            vlist();
            echo = 0;
            break;

        case HELP:
            printf("Stack:\n"
                   "\tstack     view stack contents\n"
                   "\tdup       duplicate top element\n"
                   "\tswp       swap top two elements\n"
                   "\tx pop     pop x elements from the stack\n"
                   "\tclr       clear stack.\n"
                   "Variables:\n"
                   "\tx v set   assign value x to variable v\n"
                   "\tv =       print value of variable v\n"
                   "\tans       variable, holds last evaluated value\n"
                   "\tvlist     print list of variables\n");
            echo = 0;
            break;

        case PROMPT:
            if (echo && stacklen()) {
                op1 = peek();
                setvar('{', op1);
                printf("\x1b[91m" "ans =\t%.8g\n" "\x1b[0m", op1);
            } else if (!isnan(anstmp)  &&  getvar('{') != anstmp) {
                setvar('{', anstmp);
                printf("\x1b[91m" "ans =\t%.8g\n" "\x1b[0m", anstmp);
            }
            printf(">> ");
            echo = 0;
            break;

        case INV:
            printf("Unknown command '%s'.\n", s);
            echo = 0;
            break;
        }

        if (echo && stacklen())
            anstmp = peek();  // memoize running evaluations
    }

    putchar('\n');
    return 0;
}

