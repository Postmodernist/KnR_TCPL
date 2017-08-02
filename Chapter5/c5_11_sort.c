#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINES 5000       /* max #lines to be sorted */
char *lineptr[MAXLINES];    /* pointers to text lines */

/* type of lines comparison function */
typedef int (*cmpfn_t)(const void *, const void *);

int  readlines(char *lineptr[], int maxlines);
void writelines(char *lineptr[], int nlines);
void freelines(char *lineptr[], int nlines);
void qsort_r(void *lineptr[], int lef, int right, cmpfn_t comp);
int  numcmp(const char *, const char *);


/* Sort input lines. */
int main(int argc, char *argv[]) {
    int nlines;         /* number of input lines read */
    int numeric = 0;    /* 1 if numeric sort */

    if (argc > 1 && strcmp(argv[1], "-n") == 0)
        numeric = 1;
    if ((nlines = readlines(lineptr, MAXLINES)) >= 0) {
        qsort_r((void **)lineptr, 0, nlines-1,
            (cmpfn_t)(numeric ? numcmp : strcmp));
        writelines(lineptr, nlines);
        freelines(lineptr, nlines);
        return 0;
    } else {
        printf("input too big to sort\n");
        return 1;
    }
}


/* Sort v[left]...v[right] into increasing order. */
void qsort_r(void *v[], int left, int right, cmpfn_t comp) {
    int i, last;
    void swap(void *v[], int, int);

    if (left >= right)  /* do nothing if array contains */
        return;         /* fewer than two elements */
    swap(v, left, left + (right - left) / 2);   /* move partition elem */
    last = left;                                /* to v[0] */
    for (i = left + 1; i <= right; i++)         /* partition */
        if ((*comp)(v[i], v[left]) < 0)
            swap(v, ++last, i);
    swap(v, left, last);                        /* restore partition elem */
    qsort_r(v, left, last - 1, comp);
    qsort_r(v, last + 1, right, comp);
}


/* Compare s1 and s2 numerically. */
int numcmp(const char *s1, const char *s2) {
    double v1, v2;

    v1 = atof(s1);
    v2 = atof(s2);
    if (v1 < v2)
        return -1;
    else if (v1 > v2)
        return 1;
    else
        return 0;
}


void swap(void *v[], int i, int j) {
    void *temp;

    temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}


/* ---------------------------------------------------------------------------- */
/* I/O */

#define MAXLEN 1000     /* max lenght of any input line */
int  getline_n(char *, int);


/* Read input lines. */
int readlines(char *lineptr[], int maxlines) {
    int  len, nlines;
    char *p, line[MAXLEN];

    nlines = 0;
    while ((len = getline_n(line, MAXLEN)) > 0)
        if (nlines >= maxlines || (p = malloc(len)) == NULL)
            return -1;
        else {
            line[len-1] = '\0';  /* delete newline */
            strcpy(p, line);
            lineptr[nlines++] = p;
        }
    return nlines;
}


/* Write output lines. */
void writelines(char *lineptr[], int nlines) {
    while (nlines-- > 0)
        printf("%s\n", *lineptr++);
}


/* Get line into s, return length. */
int getline_n(char s[], int lim) {
    int c, i;

    i = 0;
    while (--lim > 0 && (c = getchar()) != EOF && c != '\n')
        s[i++] = c;
    if (c == '\n')
        s[i++] = c;
    s[i] = '\0';
    return i;
}


/* Deallocate lines. */
void freelines(char *lines[], int n) {
    while (n-- > 0)
        free(*lines++);
}
