/* 
Exercise 5-18.

Make dcl recover from input errors.

-------------------------------------------------------------------------------

Recursive-descent parser. Converts declaration to words.
Parses declarations according to grammar:

declaration:
    datatype declarator

datatype:
    identifier

declarator:
    direct-declarator
    pointer direct-declarator

direct-declarator:
    identifier
    ( declarator )
    direct-declarator []
    direct-declarator [ constant-expression ]
    direct-declarator ()

pointer:
    *
    * pointer

*/

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define MAXTOKEN 100

enum { ERROR, OK, NAME, PARENS, BRACKETS };

int  dcl(void);
int  dirdcl(void);
int  gettoken(void);
int  tokentype;             /* type of last token */
char token[MAXTOKEN];       /* last token string */
char name[MAXTOKEN];        /* identifier name */
char datatype[MAXTOKEN];    /* data type = char, int, etc. */
char out[1000];             /* output string */
extern int bufp;

void cleanup() {
    if (tokentype == ERROR)
        tokentype = '\n';
    /* flush the rest of the input string */
    while (tokentype != EOF && tokentype != '\n')
        gettoken();
    out[0] = '\0';
    bufp = 0;
}

/* Main. */
int main() {
    while (tokentype != EOF && gettoken() != EOF) {
        if (tokentype == '\n')
            continue;
        else if (tokentype == ERROR) {
            cleanup();
            continue;
        } else if (tokentype != NAME) {
            printf("dcl: error: expected datatype\n");
            cleanup();
            continue;
        }

        strcpy(datatype, token);  /* first token on line is datatype */
        
        if (dcl() == ERROR) {
            cleanup();
            continue;
        } else if (tokentype != '\n') {
            printf("dcl: syntax error\n");
            cleanup();
            continue;
        }

        printf("    %s: %s %s\n", name, out, datatype);
        out[0] = '\0';
    }

    return 0;
}


/* Parse a declarator. */
int dcl() {
    int ns;

    for (ns = 0; gettoken() == '*';)    /* count *'s */
        ns++;
    if (tokentype == ERROR)
        return ERROR;
    if (dirdcl() == ERROR)
        return ERROR;
    while (ns-- > 0)
        strcat(out, " pointer to");

    return OK;
}

/* Parse a direct declarator. */
int dirdcl() {
    if (tokentype == '(') {             /* ( dcl ) */
        if (dcl() == ERROR)
            return ERROR;
        if (tokentype != ')') {
            printf("dcl: error: missing ')'\n");
            return ERROR;
        }

    } else if (tokentype == NAME) {     /* variable name */
        strcpy(name, token);

    } else {
        printf("dcl: error: expected name or (dcl)\n");
        return ERROR;
    }

    while (gettoken() == PARENS || tokentype == BRACKETS)
        if (tokentype == PARENS)
            strcat(out, " function returning");
        else {
            strcat(out, " array");
            strcat(out, token);
            strcat(out, " of");
        }

    if (tokentype == ERROR)
        return ERROR;

    return OK;
}

/* Return next token. */
int gettoken() {
    int c, getch(void);
    void ungetch(int);
    char *p = token;

    while ((c = getch()) == ' ' || c == '\t');
    
    if (c == '(') {
        if ((c = getch()) == ')') {
            strcpy(token, "()");
            return tokentype = PARENS;
        } else {
            ungetch(c);
            return tokentype = '(';
        }
    
    } else if (c == '[') {
        for (*p++ = c; (c = getch()) != ']'; *p++ = c)
            if (c == '\n' || c == EOF) {
                printf("dcl: error: expected ']'\n");
                return tokentype = ERROR;
            }
        *p++ = c;
        *p = '\0';
        return tokentype = BRACKETS;
    
    } else if (isalpha(c)) {
        for (*p++ = c; isalnum(c = getch()); )
            *p++ = c;
        *p = '\0';
        ungetch(c);
        return tokentype = NAME;

    } else
        return tokentype = c;
}


/* ---------------------------------------------------------------------------- */
/* Getch / ungetch */

#define BUFSIZE 4
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
