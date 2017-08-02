/* Return <0 if s<t, 0 if s==t, >0 if s>t. */
int strcmp_n(char *s, char *t) {
    int i;

    for (i = 0; s[i] == t[i]; i++)
        if (s[i] == '\0')
            return 0;
    return s[i] - t[i];
}

/* Return <0 if s<t, 0 if s==t, >0 if s>t. */
int strcmp_n(char *s, char *t) {
    for (; *s == *t; s++, i++)
        if (*s == '\0')
            return 0;
    return *s - *t;
}
