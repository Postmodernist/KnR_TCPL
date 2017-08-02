/*
Exercise 5-14.

Modify the sort program to handle a -r flag, which indicates sorting in reverse
(decreasing) order. Be sure that -r works with -n.

Exercise 5-15.

Add the option -f to fold upper and lower case together, so that case
distincions a not made during sorting; for example, a and A compare equal.

Exercise 5-16.

Add the -d ("directory order") option, which makes comparisons only on letters,
numbers and blanks. Make sure it works in conjunction with -f.

Exercise 5-17.

Add a field-handling capability, so sorting may be done on fields within lines,
each field sorted according to an independent set of options. (The index for
this book was sorted with -df for the index category and -n for the page
numbers.)
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_BUF_SIZE  1024     /* size of line read buffer */
#define LINES_MAX      5000     /* max number of lines to be sorted */
#define FIELDS_MAX       10     /* max number of fields */
#define MSORT(x, l, r, c)  { void *tmp[LINES_MAX]; msort_r(x, tmp, l, r, c); }

struct Options {
    unsigned int enabled : 1;       /* options are enabled (for fields) */
    unsigned int numeric : 1;       /* numeric sort */
    unsigned int reverse : 1;       /* reverse sort */
    unsigned int folding : 1;       /* fold upper and lower case together */
    unsigned int directory : 1;     /* "directory order" comparison */
} glob;

typedef struct Field {
    int beg;                /* beginning of a field */
    int end;                /* end of a field */
    struct Options opt;     /* field sorting options (override global) */
} field_t;

char    *lineptr[LINES_MAX];    /* pointers to text lines */
field_t fields[FIELDS_MAX];     /* fields array */
int     nfields;                /* number of fields */

typedef int (*cmpfn_t)(const void *, const void *);  /* type of comparison function */

int     lines_read(const char *, char **, int);
void    lines_write(char **, int);
void    lines_free(char **, int);
void    msort_r(void **, void **, int, int, cmpfn_t);
cmpfn_t get_comp(struct Options *);
field_t get_field(const char *);


/* Main. */
int main(int argc, const char *argv[]) {
    const char *fname = NULL;   /* input file name */
    int nlines = 0;             /* number of input lines read */

    if (argc == 1) {
        printf("Print sorted lines of text file.\n"
               "Usage: sort [-OPTIONS] FILE\n"
               "Options:\n"
               "  -n                    numeric sorting (default: lexicographical)\n"
               "  -r                    reverse sorting\n"
               "  -f                    fold lower and upper case together\n"
               "  -d                    \"directory order\" (only letters, numbers and blanks)\n"
               "  -k KEYDEF             sort via a key with specified location and options;\n\n"
               "KEYDEF is BEG[,END][OPTS] for beginning and end key position, both are origin 1;\n"
               "END defaults to the end of the line; OPTS override global sort options for a key.\n"
               "Multiple keys can be defined.\n");
        exit(EXIT_SUCCESS);
    }

    for (const char **arg = argv+1; *arg; arg++) {
        if (*arg[0] == '-')
            for (const char* o = *arg+1; *o; o++)
                switch (*o) {
                case 'n':
                    glob.numeric = 1;
                    break;
                case 'r':
                    glob.reverse = 1;
                    break;
                case 'f':
                    glob.folding = 1;
                    break;
                case 'd':
                    glob.directory = 1;
                    break;
                case 'k':
                    fields[nfields++] = o[1] ? get_field(o + 1) : get_field(*++arg);
                    break;
                default:
                    break;
                }
        else
            fname = *arg;
    }

    if ((nlines = lines_read(fname, lineptr, LINES_MAX)) >= 0) {
        MSORT((void **)lineptr, 0, nlines-1, get_comp(&glob));
        lines_write(lineptr, nlines);
        lines_free(lineptr, nlines);
        exit(EXIT_SUCCESS);
    } else {
        fprintf(stderr, "Error: input too big to sort\n");
        lines_free(lineptr, LINES_MAX);
        exit(EXIT_FAILURE);
    }
}


/* Read lines from file to lines. */
int lines_read(const char *fname, char *lines[], int n) {
    FILE *fp;
    char **lp = lines;
    char buf[LINE_BUF_SIZE];

    /* open input file */
    if ((fp = fopen(fname, "r")) == NULL) {
        fprintf(stderr, "Error: can't open \"%s\"\n", fname);
        exit(EXIT_FAILURE);
    }

    /* read lines */
    for (; fgets(buf, LINE_BUF_SIZE, fp); lp++) {
        if (lp - lines == n)
            return -1;
        *lp = malloc(strlen(buf) + 1);
        strcpy(*lp, buf);
    }

    fclose(fp);
    return lp - lines;
}


/* Write lines to stdout. */
void lines_write(char *lines[], int n) {
    while (n-- > 0)
        printf("%s", *lines++);
}


/* Deallocate lines. */
void lines_free(char *lines[], int n) {
    while (n-- > 0)
        free(*lines++);
}


/* Sort v[l]..v[r] into order determined by cmp. */
void msort_r(void **v, void **tmp, int l, int r, cmpfn_t cmp) {
    void    fieldcpy(char *, char *, struct Field);
    cmpfn_t cmpfn;
    int     i, j, k, m, f, test;
    char    field1[LINE_BUF_SIZE], field2[LINE_BUF_SIZE];

    if (l >= r)
        return;
    
    /* subdivide */
    m = l + (r - l)/2;
    msort_r(v, tmp, l, m, cmp);
    msort_r(v, tmp, m + 1, r, cmp);

    /* merge */
    memcpy(tmp + l, v + l, (r - l + 1) * sizeof(void*));
    i = l;      /* left partition index */
    j = m + 1;  /* right partition index */
    for (k = l; k <= r; k++)
        /* dump the rest of left side */
        if (j > r) {
            memcpy(v + k, tmp + i, (m - i + 1) * sizeof(void *));
            break;
        /* dump the rest of right side */
        } else if (i > m) {
            memcpy(v + k, tmp + j, (r - j + 1) * sizeof(void *));
            break;
        /* compare */
        } else {
            f = 0;
            /* compare fields first */
            if (nfields)
                for (; f < nfields; f++) {
                    /* get fields content */
                    fieldcpy(field1, tmp[i], fields[f]);
                    fieldcpy(field2, tmp[j], fields[f]);
                    /* get comparison function */
                    if (fields[f].opt.enabled)
                        cmpfn = get_comp(&fields[f].opt);
                    else
                        cmpfn = cmp;
                    /* compare */
                    test = (*cmpfn)(field1, field2);
                    if (test < 0) {
                        v[k] = tmp[i++];
                        break;
                    } else if (test > 0) {
                        v[k] = tmp[j++];
                        break;
                    }
                }
            /* then compare lines */
            if (f == nfields) {
                if ((*cmp)(tmp[i], tmp[j]) <= 0)
                    v[k] = tmp[i++];
                else
                    v[k] = tmp[j++];
            }
        }
}


/* Copy field defined by fld from src to dst. */
void fieldcpy(char *dst, char *src, field_t fld) {
    int lenf;
    int beg = fld.beg - 1;
    int end = fld.end - 1;
    
    memset(dst, 0, LINE_BUF_SIZE);
    if (beg < strlen(src)) {
        if (fld.end) {
            lenf = end - beg + 1;
            strncpy(dst, src + beg, lenf);
            dst[LINE_BUF_SIZE - 1] = '\0';
        } else
            strcpy(dst, src + beg);
    }
}


/* Parse field specifier string and return Field object. */
field_t get_field(const char *s) {
    int isoption(int);
    const char *sp = s;
    struct Field fld = {0, 0, {0, 0, 0, 0, 0}};

    if (s == NULL || !isdigit(*s)) {
        printf("Error: invalid number at field start: %s\n", s);
        exit(EXIT_FAILURE);
    }

    /* get beginning of the field */
    fld.beg = atoi(sp);
    for (; isdigit(*sp); sp++);
    if (*sp == '\0')
        return fld;

    /* get end of the field */
    if (*sp == ',') {
        sp++;
        if (sp == NULL || !isdigit(*sp)) {
            printf("Error: invalid number after ',': %s\n", s);
            exit(EXIT_FAILURE);
        }

        fld.end = atoi(sp);
        if (fld.end < fld.beg) {
            printf("Error: END is less than BEG: %s\n", s);
            exit(EXIT_FAILURE);
        }
        for (; isdigit(*sp); sp++);
        if (*sp == '\0')
            return fld;
    }

    /* get field sorting options */
    if (isoption(*sp)) {
        fld.opt.enabled = 1;
        for (; isoption(*sp); sp++)
            switch (*sp) {
            case 'n': fld.opt.numeric = 1; break;
            case 'r': fld.opt.reverse = 1; break;
            case 'f': fld.opt.folding = 1; break;
            case 'd': fld.opt.directory = 1; break;
            }
        if (*sp) {
            printf("Error: stray character in field spec: %s\n", s);
            exit(EXIT_FAILURE);
        } else
            return fld;

    } else {
        printf("Error: invalid character in field spec: %s\n", s);
        exit(EXIT_FAILURE);
    }
}

int isoption(int c) {
    return c == 'n' || c == 'r' || c == 'f' || c == 'd';
}


/* ---------------------------------------------------------------------------- */
/* Comparison methods */

char *touppers(char *);
char *todirs(char *);
char *toupperdirs(char *);


/* Compare s1 and s2 numerically. */
int numcmp(char *s1, char *s2) {
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


/* Compare s1 and s2 numerically (reverse). */
int numcmp_rev(char *s1, char *s2) {
    return -numcmp(s1, s2);
}


/* Compare s1 and s2 lexicogaphically (reverse). */
int strcmp_rev(char *s1, char *s2) {
    return -strcmp(s1, s2);
}


/* Compare s1 and s2 lexicogaphically (folding). */
int strcmp_f(char *s1, char *s2) {
    s1 = touppers(s1);
    s2 = touppers(s2);
    int ret = strcmp(s1, s2);
    free(s1);
    free(s2);
    return ret;
}


/* Compare s1 and s2 lexicogaphically (directory). */
int strcmp_d(char *s1, char *s2) {
    s1 = todirs(s1);
    s2 = todirs(s2);
    int ret = strcmp(s1, s2);
    free(s1);
    free(s2);
    return ret;
}


/* Compare s1 and s2 lexicogaphically (folding, directory). */
int strcmp_fd(char *s1, char *s2) {
    s1 = toupperdirs(s1);
    s2 = toupperdirs(s2);
    int ret = strcmp(s1, s2);
    free(s1);
    free(s2);
    return ret;
}


/* Compare s1 and s2 lexicogaphically (reverse, folding). */
int strcmp_rf(char *s1, char *s2) {
    return -strcmp_f(s1, s2);
}


/* Compare s1 and s2 lexicogaphically (reverse, directory). */
int strcmp_rd(char *s1, char *s2) {
    return -strcmp_d(s1, s2);
}


/* Compare s1 and s2 lexicogaphically (reverse, folding, directory). */
int strcmp_rfd(char *s1, char *s2) {
    s1 = toupperdirs(s1);
    s2 = toupperdirs(s2);
    int ret = -strcmp_d(s1, s2);
    free(s1);
    free(s2);
    return ret;
}


/* Return uppercase variant of input string. */
char *touppers(char *s) {
    int  len = strlen(s);
    char *tmp = malloc(len + 1);

    for (int i = 0; i < len; i++)
        tmp[i] = toupper(s[i]);
    tmp[len] = '\0';
    return tmp;
}


/* Return "directory" variant of input string. */
char *todirs(char *s) {
    char *tmp = malloc(strlen(s) + 1);
    char *tp = tmp, *sp = s;

    for (; *sp; sp++)
        if (isalnum(*sp) || isspace(*sp))
            *tp++ = *sp;
    *tp = '\0';
    return tmp;
}


/* Return uppercase and "directory" variant of input string. */
char *toupperdirs(char *s) {
    char *tmp = malloc(strlen(s) + 1);
    char *tp = tmp, *sp = s;

    for (; *sp; sp++)
        if (isalnum(*sp) || isspace(*sp))
            *tp++ = toupper(*sp);
    *tp = '\0';
    return tmp;
}


/* Return comparison method according to options. */
cmpfn_t get_comp(struct Options *opt) {
    unsigned int flags = (opt->numeric << 3) +
                         (opt->reverse << 2) +
                         (opt->folding << 1) +
                          opt->directory;
    switch (flags) {  /* nrfd */
    case 0b0000: return (cmpfn_t)strcmp;
    case 0b0001: return (cmpfn_t)strcmp_d;
    case 0b0010: return (cmpfn_t)strcmp_f;
    case 0b0011: return (cmpfn_t)strcmp_fd;
    case 0b0100: return (cmpfn_t)strcmp_rev;
    case 0b0101: return (cmpfn_t)strcmp_rd;
    case 0b0110: return (cmpfn_t)strcmp_rf;
    case 0b0111: return (cmpfn_t)strcmp_rfd;
    case 0b1000:
    case 0b1001:
    case 0b1010:
    case 0b1011: return (cmpfn_t)numcmp;
    case 0b1100:
    case 0b1101:
    case 0b1110:
    case 0b1111: return (cmpfn_t)numcmp_rev;
    default:
        printf("Error: invalid flags (%du)", flags);
        exit(EXIT_FAILURE);
    }
    return NULL;
}
