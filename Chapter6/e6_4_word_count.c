/*
Exercise 6-4.

Write a program that prints the distinct words in its input sorted into
decreasing order of frequency of occurence. Precede each word by its count.
*/

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* The tree node */
struct tnode {
    char         *word;     /* word */
    int          count;     /* number of occurences */
    struct tnode *left;
    struct tnode *right;
};

const char *noise_words[] = {
    "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "a", "about", "after", "all", "also", "an",
    "and", "another", "any", "are", "as", "at", "b", "be", "because", "been", "before", "being",
    "between", "both", "but", "by", "c", "came", "can", "come", "could", "d", "did", "do", "e",
    "each", "f", "for", "from", "g", "get", "got", "h", "had", "has", "have", "he", "her", "here",
    "him", "himself", "his", "how", "i", "if", "in", "into", "is", "it", "j", "k", "l", "like",
    "m", "make", "many", "me", "might", "more", "most", "much", "must", "my", "n", "never", "now",
    "o", "of", "on", "only", "or", "other", "our", "out", "over", "p", "q", "r", "s", "said",
    "same", "see", "should", "since", "some", "still", "such", "t", "take", "than", "that", "the",
    "their", "them", "then", "there", "these", "they", "this", "those", "through", "to", "too",
    "u", "under", "up", "v", "very", "w", "was", "way", "we", "well", "were", "what", "where",
    "which", "while", "who", "with", "would", "x", "y", "you", "your", "z" };

#define MAXLINE         1000
#define MAXWORD         100
#define NWLEN           (sizeof(noise_words) / sizeof(char*))
#define ISNOISEWORD(x)  inlist_s(x, noise_words, NWLEN)

int          nline;
int          getword(char *, int);
bool         inlist_s(const char *, const char **, int);
char         *tolower_s(char*);
struct tnode *tree_add_r(struct tnode *, char *);
struct tnode *tree_alloc(char *);
struct tnode *tree_addnode_r(struct tnode *, struct tnode *);
struct tnode *tree_sort_r(struct tnode *, struct tnode *);
void         tree_print_r(struct tnode *);


int main(int argc, const char **argv) {
    struct tnode *root = NULL;
    char word[MAXWORD];

    while (getword(word, MAXWORD) != EOF)
        if (isalpha(word[0]) && !ISNOISEWORD(tolower_s(word)))
            root = tree_add_r(root, word);
    root = tree_sort_r(NULL, root);
    tree_print_r(root);
}


/* Get word from stdin. */
int getword(char *s, int lim) {
    static char buf[MAXLINE + 1] = {};
    static char *pbuf, *pend;
    int wlen;

    s[0] = '\0';
    
    while (1) {
        /* read input string */
        if (!strlen(buf)) {
            if ((pbuf = fgets(buf, MAXLINE, stdin)) == NULL)
                return EOF;
            nline++;
        }
        if (strlen(buf) == MAXLINE)
            fprintf(stderr, "getword: Input buffer is full\n");
        /* search start of word */
        for ( ;isspace(*pbuf); pbuf++);
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
        fprintf(stderr, "getword: Not enough space to store word\n");
    
    /* reset buf if it's consumed */
    if (*(pbuf = pend) == '\0')
        buf[0] = '\0';
    
    return s[0];
}


/* Search itm in a sorted list (string). */
bool inlist_s(const char *itm, const char **list, int len) {
    int strcmp_pstr(const char *, const char **);

    return bsearch(itm, list, len, sizeof(char *),
        (int (*)(const void *, const void *))strcmp_pstr);
}

/* Compare s to string pointed to by ps. */
int strcmp_pstr(const char *s, const char **ps) {
    return strcmp(s, *ps);
}


/* Convert s to lower case. */
char *tolower_s(char *s) {
    char *p;
    
    for (p = s; *p; p++)
        *p = tolower(*p);
    return s;
}


/* ---------------------------------------------------------------------------- */
/* Binary tree */


/* Add a node with w, at or below p.*/
struct tnode *tree_add_r(struct tnode *p, char *w) {
    int cond;

    if (p == NULL)          /* a new word has arrived */
        p = tree_alloc(w);  /* make a new node */
    else if ((cond = strcmp(w, p->word)) == 0)
        p->count++;         /* repeated word */
    else if (cond < 0)      /* less than into left subtree */
        p->left = tree_add_r(p->left, w);
    else                    /* greater than into right subtree */
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


/* Add a node at or below p.*/
struct tnode *tree_addnode_r(struct tnode *p, struct tnode *node) {
    if (p == NULL)
        p = node;
    else if (node->count <= p->count)
        p->right = tree_addnode_r(p->right, node);
    else
        p->left = tree_addnode_r(p->left, node);
    return p;
}


/* Sort tree by count. */
struct tnode *tree_sort_r(struct tnode *p, struct tnode *node) {
    struct tnode *left, *right;

    if (node) {
        left = node->left;
        right = node->right;
        node->left = node->right = NULL;
        p = tree_addnode_r(p, node);
        tree_sort_r(p, left);
        tree_sort_r(p, right);
    }
    return p;
}


/* In-order print of tree p. */
void tree_print_r(struct tnode *p) {
    if (p) {
        tree_print_r(p->left);
        printf("%4d %s\n", p->count, p->word);
        tree_print_r(p->right);
    }
}
