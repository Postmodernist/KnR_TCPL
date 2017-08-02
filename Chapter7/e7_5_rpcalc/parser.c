/* ---------------------------------------------------------------------- */
/* Input parser */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <rpcalc.h>

#define MAXINPUT    1024
#define STR2(x)     #x
#define STR(x)      STR2(x)

char inpbuf[MAXINPUT];   // input buffer
char* ip = inpbuf;       // input parsing position


/* Initialize input buffer. */
void initinput() {
    memset(inpbuf, 0, MAXINPUT);
}


/* Get next token. */
int gettoken(char* s) {
    s[0] = '\0';

    // Read input line.
    if (!(*ip)) {
        if (fgets(inpbuf, MAXINPUT, stdin) == NULL)
            return EOF;
        ip = inpbuf;
    }

    for (; isblank(*ip); ip++);  // skip blanks
    sscanf(ip, "%s", s);         // scan token

    // Detect and extract token.
    if (*ip == '\n') {
        ip++;
        return PROMPT;                  // new prompt
    } else if (isdigit(*ip) || (*ip == '.' && isdigit(ip[1]))) {  
        for (; !isspace(*ip); ip++);
        return NUM;                 // number
    } else if (*ip == '+') {
        ip++;
        return SUM;                     // sum operator
    } else if (*ip == '-') {
        if (isdigit(ip[1]) || (ip[1] == '.' && isdigit(ip[2]))) {
            for (; !isspace(*ip); ip++);
            return NUM;                 // negative number
        } else {
            ip++;
            return SUB;                 // subtraction operator
        }
    } else if (*ip == '*') {
        ip++;
        return MUL;                     // multiplication operator
    } else if (*ip == '/') {
        ip++;
        return DIV;                     // division operator
    } else if (*ip == '%') {
        ip++;
        return MOD;                     // modulus operator
    } else if (*ip == '=') {
        ip++;
        return VPRINT;                  // print variable command
    } else if (islower(*ip)) {
        for (; !isspace(*ip); ip++);
        if (s[1] == '\0') {
            return VAR;                 // variable
        } else if (!strcmp(s, "ans")) {
            s[0] = '{', s[1] = '\0';
            return VAR;                 // ans variable
        } else if (!strcmp(s, "stack"))
            return STACK;               // stack command
        else if (!strcmp(s, "dup"))
            return DUP;                 // dup command
        else if (!strcmp(s, "swp"))
            return SWP;                 // swp command
        else if (!strcmp(s, "clr"))
            return CLR;                 // clr command
        else if (!strcmp(s, "sin"))
            return SIN;                 // sin operator
        else if (!strcmp(s, "exp"))
            return EXP;                 // exp operator
        else if (!strcmp(s, "pow"))
            return POW;                 // pow operator
        else if (!strcmp(s, "pop"))
            return POP;                 // pop command
        else if (!strcmp(s, "set"))
            return SET;                 // set command
        else if (!strcmp(s, "vlist"))
            return VLIST;               // vlist command
        else if (!strcmp(s, "help"))
            return HELP;                // help command
    }

    // Invalid token, the rest of the input string is corrupt.
    for (; *ip != EOF  &&  *ip != '\n'; ip++);
    return INV;
}
