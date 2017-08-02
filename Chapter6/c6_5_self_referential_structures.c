#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* The tree node */
struct tnode {
    char         *word;     /* text */
    int          count;     /* number of occurences */
    struct tnode *left;     /* left child */
    struct tnode *right;    /* right child */
};

#define MAXWORD 100

struct tnode *addtree_r(struct tnode *, char *);
void         treeprint_r(struct tnode *);
int          getword(char *, int);


/* Count word frequencies in input. */
int main() {
    struct tnode *root;
    char word[MAXWORD];

    root = NULL;
    while (getword(word, MAXWORD) != EOF)
        if (isalpha(word[0]))
            root = addtree_r(root, word);
    treeprint_r(root);
    return 0;
}


struct tnode *talloc(void);
char         *strdup_n(char *);

/* Add a node with w, at or below p.*/
struct tnode *addtree_r(struct tnode *p, char *w) {
    int cond;

    if (p == NULL) {    /* a new word has arrived */
        p = talloc();   /* make a new node */
        p->word = strdup_n(w);
        p->count = 1;
        p->left = p->right = NULL;
    } else if ((cond = strcmp(w, p->word)) == 0)
        p->count++;     /* repeated word */
    else if (cond < 0)  /* less than into left subtree */
        p->left = addtree_r(p->left, w);
    else                /* greater than into right subtree */
        p->right = addtree_r(p->right, w);
    return p;
}


/* In-order print of tree p. */
void treeprint_r(struct tnode *p) {
    if (p != NULL) {
        treeprint_r(p->left);
        printf("%4d %s\n", p->count, p->word);
        treeprint_r(p->right);
    }
}


/* Make a tnode. */
struct tnode *talloc(void) {
    return (struct tnode *) malloc(sizeof(struct tnode));
}


/* Make a duplicate of s. */
char *strdup_n(char *s) {
    char *p;

    p = (char *) malloc(strlen(s)+1);  /* +1 for '\0' */
    if (p != NULL)
        strcpy(p, s);
    return p;
}


/* Get next word or character from input. */
int getword(char *word, int lim) {
    int  c, getch(void);
    void ungetch(int);
    char *w = word;

    while (isspace(c = getch()))
        ;
    if (c != EOF)
        *w++ = c;
    if (!isalpha(c)) {
        *w = '\0';
        return c;
    }
    for ( ; --lim > 0; w++)
        if (!isalnum(*w = getch())) {
            ungetch(*w);
            break;
        }
    *w = '\0';
    return word[0];
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
