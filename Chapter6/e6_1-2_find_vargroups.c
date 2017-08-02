/*
Exercise 6-1.

Our version of getword does not properly handle underscores, string constants,
comments, or preprocessor control lines. Write a better version.

Exercise 6-2.

Write a program that reads a C program and prints in alphabetical order each
group of variable names that are identical in the first 6 characters, but
different somewhere thereafter. Don't count words within strings and comments.
Make 6 a parameter that can be set from the command line.
*/

#include <ctype.h>
#include <stdbool.h>
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

#define MAXLINE         1000
#define MAXWORD         100
#define DEFAULT_NCHARS  4

const char *keywords[] =
    { "auto", "break", "case", "char", "const", "continue", "default", "do", "double",
    "else", "enum", "extern", "float", "for", "goto", "if", "int", "long", "register",
    "return", "short", "signed", "sizeof", "static", "struct", "switch", "typedef",
    "union", "unsigned", "void", "volatile", "while" };

#define kwlen           (sizeof(keywords) / sizeof(char*))
#define iskeyword(x)    inlist_s(x, keywords, kwlen)

int nchars, nvars;

int          getword(char *, int);
bool         inlist_s(const char *, const char **, int);
struct tnode *tree_add_r(struct tnode *, char *);
struct tnode *tree_alloc(char *);
struct tnode *tree_addnode_r(struct tnode *, struct tnode *);
void         tree_print_r(struct tnode *);
struct tnode *tree_filter(struct tnode *);
void         tree_ravel_r(struct tnode *, struct tnode ***);


int main(int argc, const char **argv) {
    struct tnode *root;
    char word[MAXWORD];

    nchars = argc == 2 ? atoi(argv[1]) : DEFAULT_NCHARS;
    root = NULL;
    while (getword(word, MAXWORD) != EOF)
        if (isalpha(word[0]) && strlen(word) >= nchars && !iskeyword(word)) {
            root = tree_add_r(root, word);
            nvars++;
        }
    root = tree_filter(root);
    tree_print_r(root);
}


/* Get word from stdin */
int getword(char *s, int lim) {
    static char buf[MAXLINE + 1] = {};
    static char *pbuf, *pend;
    int wlen, comment = 0;

    s[0] = '\0';
    
    while (1) {
        /* read input string */
        if (!strlen(buf))
            do {
                if ((pbuf = fgets(buf, MAXLINE, stdin)) == NULL)
                    return EOF;
                /* skip white space */
                for ( ;isspace(*pbuf); pbuf++);
            /* skip preprocessor control lines */
            } while (*pbuf == '#');
        if (strlen(buf) == MAXLINE)
            fprintf(stderr, "warning: getword: Input buffer is full\n");
        /* search start of word */
        while (1) {
            /* skip multiline comment */
            if (comment) {
                for (; pbuf[1] != '\0' && !(*pbuf == '*' && pbuf[1] == '/'); pbuf++);
                if (pbuf[1] == '\0')
                    pbuf++;
                else {
                    pbuf += 2;
                    comment = 0;
                }
            }
            /* skip white space */
            for ( ;isspace(*pbuf); pbuf++);
            /* skip string constant */
            if (*pbuf == '"')
                for (pbuf++; *pbuf != '"' && *pbuf != '\0'; pbuf++);
            /* skip comment */
            else if (*pbuf == '/' && pbuf[1] == '/') {
                *pbuf = '\0';
                break;
            /* detect multiline comment */
            } else if (*pbuf == '/' && pbuf[1] == '*') {
                pbuf += 2;
                comment = 1;
            } else
                break;
        }
        /* keep reading until word or EOF is found */
        if (*pbuf == '\0')
            buf[0] = '\0';
        else
            break;
    }
    
    /* search end of word */
    for (pend = pbuf; isalnum(*pend) || *pend == '_'; pend++);
    if (!(wlen = pend - pbuf))
        wlen++, pend++;

    /* get word */
    if (wlen < lim) {
        strncpy(s, pbuf, wlen);
        s[wlen] = '\0';
    } else
        fprintf(stderr, "error: getword: Not enough space to store word\n");
    
    /* reset buf if it's consumed */
    if (*(pbuf = pend) == '\0')
        buf[0] = '\0';
    
    return s[0];
}


/* Search itm in a list (string). */
bool inlist_s(const char *itm, const char **list, int len) {
    for (int i = 0; i < len; i++)
        if (strcmp(itm, list[i]) == 0)
            return true;
    return false;
}


/* ---------------------------------------------------------------------------- */
/* Binary tree */

/* Add a node with w, at or below p.*/
struct tnode *tree_add_r(struct tnode *p, char *w) {
    int cond;

    if (p == NULL)      /* a new word has arrived */
        p = tree_alloc(w);  /* make a new node */
    else if ((cond = strcmp(w, p->word)) == 0)
        p->count++;     /* repeated word */
    else if (cond < 0)  /* less than into left subtree */
        p->left = tree_add_r(p->left, w);
    else                /* greater than into right subtree */
        p->right = tree_add_r(p->right, w);
    return p;
}


/* Make a tnode. */
struct tnode *tree_alloc(char *w) {
    struct tnode *node;

    node = malloc(sizeof(struct tnode));
    node->word = strdup(w);
    node->count = 1;
    node->left = node->right = NULL;
    return node;
}


struct tnode *tree_addnode_r(struct tnode *p, struct tnode *node) {
    int cond;
    
    /* cleanup node links */
    node->left = node->right = NULL;
    /* insert node */
    if (p == NULL)
        p = node;
    else if ((cond = strcmp(node->word, p->word)) == 0)
        p->count += node->count;
    else if (cond < 0)
        p->left = tree_addnode_r(p->left, node);
    else
        p->right = tree_addnode_r(p->right, node);
    return p;
}


/* In-order print of tree p. */
void tree_print_r(struct tnode *p) {
    if (p != NULL) {
        tree_print_r(p->left);
        printf("%4d %s\n", p->count, p->word);
        tree_print_r(p->right);
    }
}


/* Filter out unique variables. */
struct tnode *tree_filter(struct tnode *root) {
    struct tnode *nodelist[nvars], **pn, **list_end, *prev, *next, *newroot;

    /* flatten tree */
    pn = nodelist;
    tree_ravel_r(root, &pn);
    /* filter list and build a new tree */
    list_end = pn;
    pn = nodelist;
    newroot = prev = next = NULL;
    for (; pn < list_end; prev = *pn++) {
        next = pn + 1 < list_end ? pn[1] : NULL;
        if ((prev && strncmp((*pn)->word, prev->word, nchars) == 0) ||
            (next && strncmp((*pn)->word, next->word, nchars) == 0))
            newroot = tree_addnode_r(newroot, *pn);
    }
    return newroot;
}


/* Unravel bintree. */
void tree_ravel_r(struct tnode *node, struct tnode ***ppn) {
    if (node) {
        tree_ravel_r(node->left, ppn);
        *(*ppn)++ = node;
        tree_ravel_r(node->right, ppn);
    }
}
