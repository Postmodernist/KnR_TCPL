/*
Exercise 5-19.

Modify undcl so that it does not add redundant parentheses to declarations.

-------------------------------------------------------------------------------

Convert word description to declaration.

    x () * [] * () char
to
    char (*(*x())[])()
*/

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define MAXTOKEN 100

enum { ERROR, OK, NAME, PARENS, BRACKETS };

int  gettoken(void);
int  tokentype;             // type of last token
char token[MAXTOKEN];       // last token string
char out[1000];             // output string
extern int bufp;

void cleanup() {
    if (tokentype == ERROR)
        tokentype = '\n';
    // flush the rest of the input string
    while (tokentype != '\n' && tokentype != EOF)
        gettoken();
    out[0] = '\0';
    bufp = 0;
}

/* Main. */
int main() {
    int type;
    char temp[MAXTOKEN];
    char prevtoken[MAXTOKEN];

    while (tokentype != EOF && gettoken() != EOF) {
        if (tokentype == '\n')
            continue;
        else if (tokentype == ERROR) {
            cleanup();
            continue;
        } else if (tokentype != NAME) {
            printf("undcl: error: expected name\n");
            cleanup();
            continue;
        }

        strcpy(out, token);         // first token in line is a name

        if (gettoken() == '\n' || tokentype == EOF) {
            printf("undcl: error: unexpected end of declaration\n");
            cleanup();
            continue;
        } else if (tokentype == ERROR) {
            cleanup();
            continue;
        }

        while ((type = tokentype) != '\n' && type != EOF) {
            strcpy(prevtoken, token);   // save token
            if (gettoken() == ERROR) {  // get next token
                cleanup();                 
                break;
            }

            if (type == PARENS || type == BRACKETS) {
                strcat(out, prevtoken);
            } else if (type == '*' && (tokentype == PARENS || tokentype == BRACKETS)) {
                sprintf(temp, "(*%s)", out);
                strcpy(out, temp);
            } else if (type == '*') {
                sprintf(temp, "*%s", out);
                strcpy(out, temp);
            } else if (type == NAME) {
                sprintf(temp, "%s %s", prevtoken, out);  //type
                strcpy(out, temp);
            } else {
                printf("undcl: error: invalid input\n");
                cleanup();
                break;
            }
        }

        if (strlen(out))
            printf("%s\n", out);
    }

    return 0;
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
                printf("undcl: error: expected ']'\n");
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
