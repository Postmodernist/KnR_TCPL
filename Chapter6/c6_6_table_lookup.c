#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Table entry */
struct nlist {
    struct nlist *next; /* next entry in chain */
    char         *name; /* defined name */
    char         *defn; /* replacement text */
};

#define HASHSIZE 101

static struct nlist *hashtab[HASHSIZE];  /* pointer table */

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
        printf("warning: remove: key not found\n");
}


int main() {
    struct nlist *a, *b;

    install("foo", "10");
    install("bar", "99");
    a = lookup("foo");
    b = lookup("bar");
    printf("%s %s\n", a->name, a->defn);
    printf("%s %s\n", b->name, b->defn);
}
