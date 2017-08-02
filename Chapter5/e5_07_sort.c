/*
Exercise 5-7.

Rewrite readlines to store lines in an array supplied by main, rather than
calling alloc to maintain storage. How much faster is the program?
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINES    5000
#define MAXLEN      1000

#define MSORT(x, l, r)  { char *tmp[MAXLINES]; msort_r(x, tmp, l, r); }

int  readlines(const char *, char **, char (*)[MAXLEN], int);
void writelines(const char **, int);
void msort_r(char **, char **, int, int);


int main(int argc, const char *argv[]) {
    char lines[MAXLINES][MAXLEN];   /* array to store lines */
    char *plines[MAXLINES];         /* array of pointers to lines */
    int  nlines;                    /* # of lines */

    if (argc < 2) {
        fprintf(stderr, "No file name given\n");
        exit(EXIT_FAILURE);
    }
    if ((nlines = readlines(argv[1], plines, lines, MAXLINES)) >= 0) {
        MSORT(plines, 0, nlines - 1)
        writelines((const char **)plines, nlines);
    } else {
        fprintf(stderr, "Input too big to sort\n");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}


/* Read lines from file to lines. */
int readlines(const char *fname, char *plines[], char (*lines)[MAXLEN], int n) {
    int    getline_n(const char*, char*, int);
    FILE   *fp;
    char   *source, *sp, (*lp)[MAXLEN];
    size_t bufsize, slen, len;

    /* open file */
    if ((fp = fopen(fname, "r")) == NULL) {
        fprintf(stderr, "Can't open '%s'.\n", fname);
        exit(EXIT_FAILURE);
    }

    /* get file size */
    fseek(fp, 0L, SEEK_END);
    bufsize = ftell(fp);  
    if (bufsize == -1) {
        fprintf(stderr, "Failed to seek EOF of '%s'.\n", fname);
        exit(EXIT_FAILURE);
    }

    /* allocate memory */
    source = malloc(bufsize + 1);
    
    /* read file contents into memory */
    fseek(fp, 0L, SEEK_SET);
    slen = fread(source, 1, bufsize, fp);
    if (ferror(fp)) {
        fprintf(stderr, "Failed to read '%s'", fname);
        exit(EXIT_FAILURE);
    } else
        source[slen] = '\0';
    fclose(fp);

    /* read lines */
    sp = source;
    lp = lines;
    for (; sp - source < slen; sp += len + 1) {
        len = getline_n(sp, *lp, MAXLEN);
        *(plines++) = *(lp++);
        if (len < 0)
            break;
        else if (lp - lines >= MAXLINES)
            return -1;
    }
    free(source);
    return lp - lines;
}


/* Read line from src to dst. */
int getline_n(const char *src, char *dst, int n) {
    const char *p = src;
    int len;
    
    for (; !(*p == '\0' || *p == '\n'); ++p);
    len = p - src;
    if ((*p == '\0' && len == 0) || len > n)
        return -1;
    strncpy(dst, src, len);
    dst[len] = '\0';
    return len;
}


/* Write lines to stdout. */
void writelines(const char *plines[], int n) {
    while (n-- > 0)
        printf("%s\n", *(plines++));
}



/* Sort lines[l]..lines[r] into increasing order. */
void msort_r(char *plines[], char *tmp[], int l, int r) {
    int  i, j, k, m;
    char *si, *sj;

    if (l >= r)
        return;
    
    /* Subdivide */
    m = l + (r - l)/2;
    msort_r(plines, tmp, l, m);
    msort_r(plines, tmp, m + 1, r);

    /* Merge */
    memcpy(tmp + l, plines + l, (r - l + 1) * sizeof(char *));
    i = l;      /* left partition index */
    j = m + 1;  /* right partition index */
    for (k = l; k <= r; k++)
        if (j > r) {            /* dump the rest of left partition */
            memcpy(plines + k, tmp + i, (m - i + 1) * sizeof(char *));
            break;
        } else if (i > m) {     /* dump the rest of right partition */
            memcpy(plines + k, tmp + j, (r - j + 1) * sizeof(char *));
            break;
        } else {                /* append lexicographically lesser string */
            /* skip white space */
            for (si = tmp[i]; isspace(*si); ++si);
            for (sj = tmp[j]; isspace(*sj); ++sj);
            if (strcmp(si, sj) <= 0)
                plines[k] = tmp[i++];
            else
                plines[k] = tmp[j++];
        }
}
