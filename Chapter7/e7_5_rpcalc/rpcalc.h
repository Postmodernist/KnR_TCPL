#define MAXTOKEN    100

enum { INV = 1000, PROMPT, NUM, SUM, SUB, MUL, DIV, MOD, SIN, EXP, POW, STACK, DUP, SWP, CLR, POP,
       VAR, SET, VPRINT, VLIST, HELP };

// input
void initinput(void);
int gettoken(char*);

// stack
void push(double);
double pop(void);
double peek(void);
void view(void);
void flush(void);
int stacklen(void);

// variables
void initvar(void);
double getvar(char);
void setvar(char, double);
void vlist(void);
