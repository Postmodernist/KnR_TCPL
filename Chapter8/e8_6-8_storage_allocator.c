/*
Exercise 8-6.

The standard library function calloc(n, size) return a pointer to n objects of
size size, with the storage initialized to zero. Write calloc, by calling
malloc or by modifying it.

Exercise 8-7.

malloc accepts a size request without checking its plausibility; free believes
that the block it is asked to free contains a valid size field. Improve these
routines so they take more pains with error checking.

Exercise 8-8.

Write a routine bfree(p, n) that will free an arbitrary block p of n characters
into the free list maintained by malloc and free. By using bfree, a user can
add a static or external array to the free list at any time.
*/

#include <stdio.h>
#include <string.h>
#include <sys/mman.h>

#define NALLOC      1024        /* minimum #units to request */
#define MEM_MAX     65536       /* maximum heap size */

#define alloc(x)    mmap(NULL, (x), PROT_READ|PROT_WRITE, MAP_ANON|MAP_PRIVATE, -1, 0)

typedef long Align;             /* for alignment to long boundary */

typedef union header {          /* block header */
    struct {
        union header *ptr;      /* next block if on free list */
        size_t       size;      /* size of this block */
    } s;
    Align x;
} Header;

static Header base;             /* empty list to get started */
static Header *pfree = NULL;    /* start of free list */
static size_t heap_sz;


/* Put block ap in free list. */
void free_n(void *ap) {
    Header *bp, *p;

    bp = (Header*)ap - 1;       /* Rewind to block header */
    if (bp->s.size > MEM_MAX)
        fprintf(stderr, "free_n: Not a valid header\n");

    for (p = pfree; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
        if (p >= p->s.ptr && (bp > p || bp < p->s.ptr))
            break;              /* freed block at start or end of arena */

    if (bp + bp->s.size == p->s.ptr) {  /* join to upper block */
        bp->s.size += p->s.ptr->s.size;
        bp->s.ptr = p->s.ptr->s.ptr;
    } else
        bp->s.ptr = p->s.ptr;

    if (p + p->s.size == bp) {          /* join to lower block */
        p->s.size += bp->s.size;
        p->s.ptr = bp->s.ptr;
    } else
        p->s.ptr = bp;

    pfree = p;
}

/* Free an arbitrary block ptr of n characters into the free list maintained by malloc and free. */
void bfree(char *ptr, size_t n) {
    Header  *phead;
    size_t  sz;

    if (n < sizeof(Header) * 2) {       /* block is too small */
        fprintf(stderr, "bfree: Buffer is too small\n");
        return;
    }

    for (char *p = ptr; p - ptr < n; p++)
        *p = 0;

    if (pfree == NULL) {                /* no free list yet */
        base.s.ptr = pfree = &base;
        base.s.size = 0;
    }

    sz = n / sizeof(Header);
    phead = (Header*)ptr;
    phead->s.size = sz;
    free_n(phead + 1);
}

/* Ask system for more memory. */
static Header *morecore(size_t units) {
    char    *pmem;
    Header  *phead;

    if (units < NALLOC)
        units = NALLOC;

    if (units * sizeof(Header) + heap_sz > MEM_MAX)  /* heap limit reached */
        return NULL;

    pmem = alloc(units * sizeof(Header));
    if (pmem == MAP_FAILED)       /* no space at all */
        return NULL;

    phead = (Header*)pmem;
    phead->s.size = units;
    free_n(phead + 1);
    heap_sz += units * sizeof(Header);
    return pfree;
}


/* General-purpose storage allocator. */
void *malloc_n(size_t nbytes) {
    Header  *p, *pprev;
    size_t  nunits;

    nunits = (nbytes + sizeof(Header) - 1) / sizeof(Header) + 1;

    if ((pprev = pfree) == NULL) {      /* no free list yet */
        base.s.ptr = pfree = pprev = &base;
        base.s.size = 0;
    }

    for (p = pprev->s.ptr;; pprev = p, p = p->s.ptr) {
        if (p->s.size >= nunits) {      /* big enough */
            if (p->s.size == nunits)    /* exactly */
                pprev->s.ptr = p->s.ptr;
            else {                      /* allocate tail end */
                p->s.size -= nunits;
                p += p->s.size;
                p->s.size = nunits;
            }
            pfree = pprev;
            return p+1;
        }

        if (p == pfree)                 /* wrapped around free list */
            if ((p = morecore(nunits)) == NULL)
                return NULL;            /* none left */
    }
}


/* Return pointer to n objects of size sz, with the storage initialized to zero. */
void *calloc_n(size_t n, size_t sz) {
    char *mem;

    mem = malloc_n(n * sz);
    if (mem == NULL)
        return NULL;

    for (char *p = mem; p - mem < sz; p++)
        *p = 0;

    return mem;
}


int main(int argc, char const *argv[]) {
    char *s;
    int  *i;
    static char arr[100];

    /* add static array to heap pool */
    bfree(arr, sizeof(arr));

    /* test malloc_n */
    s = malloc_n(10);
    strcpy(s, "Hello!");
    printf("%s\n", s);

    /* test calloc_n */
    i = calloc_n(5, sizeof(int));
    i[0] = 3;
    i[1] = 99;
    printf("%d %d %d\n", i[0], i[1], i[2]);

    free_n(s);
    free_n(i);

    /* try to allocate more than maximum heap size */
    if (malloc_n(100000) == NULL)
        printf("OK -- Not allocated\n");

    return 0;
}
