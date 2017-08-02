/* Reverse Polish calculator. */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>     /* atof */

#define MAXOP   100     /* max size of operand or operator */
#define MAXVAL  100     /* max depth of val stack */
#define BUFSIZE 100     /* ungetch buffer size */
#define NUMBER  '0'     /* signal that a number was found */

int    getop(char*);
void   push(double);
double pop(void);


int main() {
    int type;
    double op2;
    char s[MAXOP];

    while ((type = getop(s)) != EOF) {
        switch (type) {
        case NUMBER:
            push(atof(s));
            break;
        case '+':
            push(pop() + pop());
            break;
        case '*':
            push(pop() * pop());
            break;
        case '-':
            op2 = pop();
            push(pop() - op2);
            break;
        case '/':
            op2 = pop();
            if (op2 != 0.0)
                push(pop() / op2);
            else
                fprintf(stderr, "error: Zero divisor\n");
            break;
        case '\n':
            printf("\t%.8g\n", pop());
            break;
        default:
            fprintf(stderr, "error: Unknown command '%s'\n", s);
            break;
        }
    }

    return 0;
}


/* ---------------------------------------------------------------------------- */
/* Stack. */

int sp = 0;             /* next free stack position */
double val[MAXVAL];     /* value stack */


/* Push f onto value stack. */
void push(double f) {
    if (sp < MAXVAL)
        val[sp++] = f;
    else
        fprintf(stderr, "error: Stack full, can't push %g\n", f);
}


/* Pop and return top value from stack. */
double pop(void) {
    if (sp > 0)
        return val[--sp];
    else {
        fprintf(stderr, "error: Stack empty\n");
        return 0.0;
    }
}


/* ---------------------------------------------------------------------------- */
/* I/O. */

int  getch(void);
void ungetch(int);

/* Get next operator or numeric operand. */
int getop(char *s) {
    int i, c;

    do {
        s[0] = c = getch();
    } while (c == ' ' || c == '\t');
    s[1] = '\0';

    if (!isdigit(c) && c != '.')
        return c;       /* not a number */

    i = 0;
    if (isdigit(c))     /* collect integer part */
        while (isdigit(s[++i] = c = getch()));
    if (c == '.')       /* collect fraction part */
        while (isdigit(s[++i] = c = getch()));
    s[i] = '\0';

    if (c != EOF)
        ungetch(c);

    return NUMBER;
}


/* ---------------------------------------------------------------------------- */
/* Getch / ungetch */

char buf[BUFSIZE];
int  bufp = 0;

int getch(void) {
    return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c) {
    if (bufp < BUFSIZE)
        buf[bufp++] = c;
    else
        fprintf(stderr, "ungetch: Too many characters\n");
}
