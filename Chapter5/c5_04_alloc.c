#include <stdio.h>

#define ALLOCSIZE 10000  /* size of available space */

static char allocbuf[ALLOCSIZE];  /* storage for alloc */
static char *allocp = allocbuf;   /* next free position */

/* Return pointer to n characters. */
char *alloc(int n) {
    if (allocbuf + ALLOCSIZE - allocp >= n) {  /* it fits */
        allocp += n;
        return allocp - n;  /* old p */
    } else  /* not enough room */
        return NULL;
}

/* Free storage pointed to by p. */
void afree(char *p) {
    if (p >= allocbuf && p < allocbuf + ALLOCSIZE)
        allocp = p;
}

