/*
Exercise 6-3.

Write a cross-referencer that prints a list of all words in a document, and,
for each word, a list of the line numbers on which it occurs. Remove noise
words like "the", "and", and so on.
*/

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* Line numbers tree node */
struct tnode_l {
    int             line;   /* line number */
    struct tnode_l* left;
    struct tnode_l* right;
};

/* The tree node */
struct tnode_w {
    char           *word;   /* word */
    int            count;   /* number of occurences */
    struct tnode_l *lines;  /* list of line numbers */
    struct tnode_w *left;
    struct tnode_w *right;
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

int            nline;
int            getword(char *, int);
bool           inlist_s(const char *, const char **, int);
char           *tolower_s(char*);
struct tnode_w *tree_add_w_r(struct tnode_w *, char *);
struct tnode_l *tree_add_l_r(struct tnode_l *);
struct tnode_w *tree_alloc_w(char *);
struct tnode_l *tree_alloc_l();
void           tree_print_w_r(struct tnode_w *);
void           tree_print_l_r(struct tnode_l *, int);


int main(int argc, const char *argv[]) {
    struct tnode_w *root = NULL;
    char word[MAXWORD];

    while (getword(word, MAXWORD) != EOF)
        if (isalpha(word[0]) && !ISNOISEWORD(tolower_s(word)))
            root = tree_add_w_r(root, word);
    tree_print_w_r(root);
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
struct tnode_w *tree_add_w_r(struct tnode_w *p, char *w) {
    int cond;

    if (p == NULL)      /* a new word has arrived */
        p = tree_alloc_w(w);  /* make a new node */
    else if ((cond = strcmp(w, p->word)) == 0) {
        p->count++;     /* repeated word */
        tree_add_l_r(p->lines);
    }
    else if (cond < 0)  /* less than into left subtree */
        p->left = tree_add_w_r(p->left, w);
    else                /* greater than into right subtree */
        p->right = tree_add_w_r(p->right, w);
    return p;
}


/* Add a node with nline, at or below p.*/
struct tnode_l *tree_add_l_r(struct tnode_l *p) {
    if (p == NULL)
        p = tree_alloc_l();
    if (nline < p->line)
        p->left = tree_add_l_r(p->left);
    else if ((nline > p->line))
        p->right = tree_add_l_r(p->right);
    return p;
}


/* Make a tnode_w. */
struct tnode_w *tree_alloc_w(char *w) {
    struct tnode_w* node;

    node = malloc(sizeof(struct tnode_w));
    node->word = strdup(w);
    node->count = 1;
    node->lines = tree_alloc_l();
    node->left = node->right = NULL;
    return node;
}


/* Make a tnode_l. */
struct tnode_l *tree_alloc_l() {
    struct tnode_l* node;

    node = malloc(sizeof(struct tnode_l));
    node->line = nline;
    node->left = node->right = NULL;
    return node;
}


/* In-order print of tree p. */
void tree_print_w_r(struct tnode_w *p) {
    if (p) {
        tree_print_w_r(p->left);
        printf("%s ", p->word);
        for (int i = 0; i < 20 - (int)strlen(p->word); i++)
            putchar(' ');
        tree_print_l_r(p->lines, 1);
        putchar('\n');
        tree_print_w_r(p->right);
    }
}


/* In-order print of tree p. */
void tree_print_l_r(struct tnode_l *p, int initial) {
    if (p) {
        tree_print_l_r(p->left, 0);
        if (!initial)
            printf(", %d", p->line);
        else
            printf("%d", p->line);
        tree_print_l_r(p->right, 0);
    }
}
