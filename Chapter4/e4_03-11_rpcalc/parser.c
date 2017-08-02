/* ---------------------------------------------------------------------- */
/* Input parser */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <rpcalc.h>

#define MAXINPUT    1024

char inpbuf[MAXINPUT];   // input buffer
char* ip = inpbuf;       // input parsing position


void getinput(void);
void collectnum(char*, char*);
void collectsym(char*, char*);


/* Initialize input buffer. */
void initinput() {
    memset(inpbuf, 0, MAXINPUT);
}


/* Get next token. */
int gettoken(char s[]) {
    if (!(*ip))
        getinput();  // read new input line

    // Skip white space.
    for (; *ip == ' ' || *ip == '\t'; ip++);

    char* tok = s;
    char* maxtok = s + MAXTOKEN - 1;

    // Detect and extract token.
    if (*ip == EOF)
        return EOF;                     // session end
    else if (*ip == '\n') {
        ip++;
        return PROMPT;                  // new prompt
    } else if (isdigit(*ip) || *ip == '.') {  
        *(tok++) = *(ip++);
        if (isdigit(*(tok-1))  ||  isdigit(*(tok++) = *(ip++))) { 
            collectnum(tok, maxtok);
            return NUM;                 // number
        }
    } else if (*ip == '+') {
        ip++;
        return SUM;                     // sum operator
    } else if (*ip == '-') {
        if (!isdigit(*(++ip)) && *ip != '.')
            return SUB;                 // subtraction operator
        else {
            *(tok++) = '-', *(tok++) = *(ip++);
            if (isdigit(*(tok-1))  ||  isdigit(*(tok++) = *(ip++))) { 
                collectnum(tok, maxtok);
                return NUM;             // negative number
            }
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
        *(tok++) = *(ip++);
        collectsym(tok, maxtok);
        if (s[1] == '\0')
            return VAR;                 // variable
        else if (!strcmp(s, "ans")) {
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

    // Invalid token, the rest of the string is corrupt.
    for (; (*tok = *ip) != '\n'  &&  *ip != EOF  &&  tok < maxtok; tok++, ip++);
    if (tok == maxtok)
        for (; *ip != EOF  &&  *ip != '\n'; ip++);  // skip the rest of the input
    *tok = '\0';
    return INV;
}


/* Read input line. Returns pointer to input array, or NULL if nothing read. */
void getinput() {
    char* inp_max = inpbuf + MAXINPUT - 2;
    for (ip = inpbuf; (*ip = getchar()) != '\n'  &&  *ip != EOF  &&  ip < inp_max; ip++);
    
    if (ip == inp_max) {
        *ip = '\n';
        printf("Input buffer overflow, input string truncated.");
    }

    *(ip + 1) = '\0';
    ip = inpbuf;
}


/* Collect number and store in s[]. */
void collectnum(char* tok, char* max) {
    for (; isdigit(*tok = *ip)  &&  tok < max; tok++, ip++);  // collect integer part
    
    if (*tok == '.'  &&  tok < max) {
        tok++, ip++;
        for (; isdigit(*tok = *ip)  &&  tok < max; tok++, ip++);  // collect fraction part
    }

    if (tok == max) {
        for (; isdigit(*ip) || *ip == '.'; ip++); // skip the rest of the number
        printf("Number exceeds maximum token length, truncated.\n");
    }

    *tok = '\0';
}


/* Collect symbol and store in s[]. */
void collectsym(char* tok, char* max) {
    for (; islower(*tok = *ip)  &&  tok < max; tok++, ip++);

    if (tok == max) {
        for (; islower(*ip); ip++); // skip the rest of the symbol
        printf("Symbol exceeds maximum token length, truncated.\n");
    }

    *tok = '\0';
}
