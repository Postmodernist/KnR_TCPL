/*
Exercise 3-2.

Write a function escape(s, t) that converts characters like newline and tab
into visible escape sequences like \n and \t as it copies the string t to s.
Use a switch. Write a function for the other direction as well, converting
escape sequences into the real characters.
*/

#include <stdio.h>

/* Replace invisible characters with escape sequences. */
void escape(const char s[], char t[]) {
    int i, j;

    for (i = 0, j = 0; s[i]; i++, j++)
        switch (s[i]) {
        case '\b':
            t[j++] = '\\';
            t[j] = 'b';
            break;
        case '\f':
            t[j++] = '\\';
            t[j] = 'f';
            break;
        case '\n':
            t[j++] = '\\';
            t[j] = 'n';
            break;
        case '\r':
            t[j++] = '\\';
            t[j] = 'r';
            break;
        case '\t':
            t[j++] = '\\';
            t[j] = 't';
            break;
        case '\v':
            t[j++] = '\\';
            t[j] = 'v';
            break;
        default:
            t[j] = s[i];
        }
    t[j] = '\0';
}

/* Replace escape sequences with real characters. */
void descape(const char s[], char t[]) {
    int i, j;

    for (i = 0, j = 0; s[i]; i++, j++)
        if (s[i] == '\\')
            switch (s[i+1]) {
            case 'b':
                t[j] = '\b';
                i++;
                break;
            case 'f':
                t[j] = '\f';
                i++;
                break;
            case 'n':
                t[j] = '\n';
                i++;
                break;
            case 'r':
                t[j] = '\r';
                i++;
                break;
            case 't':
                t[j] = '\t';
                i++;
                break;
            case 'v':
                t[j] = '\v';
                i++;
                break;
            default:
                t[j] = s[i];
            }
        else
            t[j] = s[i];
    t[j] = '\0';
}


int main(int argc, const char *argv[]) {
    int c, i;
    char s[BUFSIZ], t[BUFSIZ];

    for (i = 0; (c = getchar()) != EOF; i++)
        s[i] = c;
    s[i] = '\0';
    if (argc == 2 && argv[1][0] == 'e')
        escape(s, t);
    else
        descape(s, t);
    printf("%s", t);
}

