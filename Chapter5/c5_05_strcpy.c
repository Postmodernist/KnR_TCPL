/* Copy t to s; array subscript version. */
void strcpy_n(char *s, char *t) {
    int i;

    i = 0;
    while ((s[i] = t[i]) != '\0')
        i++;
}

/* Copy t to s; pointer version 1. */
void strcpy_n(char *s, char *t) {
    while ((*s = *t) != '\0') {
        s++;
        t++;
    }
}

/* Copy t to s; pointer version 2. */
void strcpy_n(char *s, char *t) {
    while ((*s++ = *t++) != '\0')
        ;
}

/* Copy t to s; pointer version 3. */
void strcpy_n(char *s, char *t) {
    while ((*s++ = *t++))
        ;
}
