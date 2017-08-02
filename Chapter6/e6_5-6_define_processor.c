/*
Exercise 6-5.

Write a function undef that will remove a name and definition from the table
maintained by lookup and install.

Exercise 6-6.

Implement a simple version of the #define processor (i.e., no arguments)
suitable for use with C programs, based on the routines of this section. You
may also find getch and ungetch helpful.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Table entry */
struct nlist {
    struct nlist *next;     /* next entry in chain */
    char         *name;     /* defined name */
    char         *defn;     /* replacement text */
};

#define MAXLINE         1000
#define MAXWORD         100
#define HASHSIZE        101

static struct nlist *hashtab[HASHSIZE];  /* pointer table */

int          getword(char *, int);
struct nlist *lookup(char *);
struct nlist *install(char *, char *);
void         undef(char *);


int main() {
    char word[MAXWORD], name[MAXWORD], defn[MAXWORD], prev;
    struct nlist *tmp;

    prev = '\0';
    while (getword(word, MAXWORD) != EOF) {
        /* process #define */
        if ((prev == '\n' || prev == '\0') && word[0] == '#') {
            if (getword(word, MAXWORD) != EOF && strcmp(word, "define") == 0) {
                if (getword(word, MAXWORD) != EOF && isspace(word[0]) &&
                    getword(name, MAXWORD) != EOF && isalpha(name[0]) &&
                    getword(word, MAXWORD) != EOF && isspace(word[0]) &&
                    getword(defn, MAXWORD) != EOF && isalnum(defn[0])) {
                    install(name, defn);
                } else {
                    fprintf(stderr, "main: Invalid definition\n");
                    exit(EXIT_FAILURE);
                }
            }
            while (word[0] != '\n')
                if (getword(word, MAXWORD) == EOF)
                    exit(EXIT_SUCCESS);
            prev = word[0];
        /* process words */
        } else {
            /* substitute names if needed */
            if (isalpha(word[0]) && (tmp = lookup(word)))
                printf("%s", tmp->defn);
            else
                printf("%s", word);
            prev = word[0];
        }
    }
    exit(EXIT_SUCCESS);
}


/* Get word from stdin */
int getword(char *s, int lim) {
    static char buf[MAXLINE + 1] = {};
    static char *pbuf, *pend;
    int wlen;

    s[0] = '\0';
    /* read input string */
    if (!strlen(buf))
        if ((pbuf = fgets(buf, MAXLINE, stdin)) == NULL)
            return EOF;
    if (strlen(buf) == MAXLINE)
        fprintf(stderr, "getword: Input buffer is full\n");
    /* search end of word or white space */
    if (isspace(*pbuf))
        for (pend = pbuf; isspace(*pend); pend++);
    else
        for (pend = pbuf; isalnum(*pend) || *pend == '_'; pend++);
    if (!(wlen = pend - pbuf))
        wlen++, pend++;
    /* get word */
    if (wlen < lim) {
        strncpy(s, pbuf, wlen);
        s[wlen] = '\0';
    } else
        fprintf(stderr, "getword: Not enough space to store word\n");
    /* reset buf if it's consumed */
    if (*(pbuf = pend) == '\0')
        buf[0] = '\0';
    return s[0];
}


/* ---------------------------------------------------------------------------- */
/* Hash table */

/* 
hash
    
    Form hash value for string s.
*/
unsigned hash(char *s) {
    unsigned hashval;

    for (hashval = 0; *s != '\0'; s++)
        hashval = *s + 31 * hashval;
    return hashval % HASHSIZE;
}

/*
lookup

    Look for s in hashtab.
*/
struct nlist *lookup(char *s) {
    struct nlist *np;

    for (np = hashtab[hash(s)]; np != NULL; np = np->next)
        if (strcmp(s, np->name) == 0)
            return np;  /* found */
    return NULL;        /* not found */
}

/*
install

    Put (name, defn) in hashtab.
*/
struct nlist *install(char *name, char *defn) {
    struct nlist *np;
    unsigned hashval;

    if ((np = lookup(name)) == NULL) {  /* not found */
        np = malloc(sizeof(*np));
        if (np == NULL || (np->name = strdup(name)) == NULL)
            return NULL;
        hashval = hash(name);
        np->next = hashtab[hashval];
        hashtab[hashval] = np;
    } else                              /* already there */
        free(np->defn);                 /* free previous defn */
    if ((np->defn = strdup(defn)) == NULL)
        return NULL;
    return np;
}

/*
undef

    Remove (name, defn) from hashtab.
*/
void undef(char *name) {
    struct nlist *np, *prev;
    unsigned hashval;

    hashval = hash(name);
    prev = np = hashtab[hashval];
    if (np && strcmp(name, np->name) == 0) {
        hashtab[hashval] = NULL;
        return;
    }
    for (; np && strcmp(name, np->name) != 0; prev = np, np = np->next);
    if (np) {
        prev->next = np->next;
        free(np->defn);
        free(np);
    } else
        fprintf(stderr, "undef: Key not found\n");
}
