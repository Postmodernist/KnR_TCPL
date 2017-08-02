/*
Exercise 5-6.

Rewrite appropriate programs from earlier chapters and exercises with pointers
with pointers instead of array indexing. Good possibilities include getline
(Chapter 1 and 4), atoi, itoa, and their variants (Chapters 2, 3, and 4),
reverse (Chapter 3), and strindex and getop (Chapter 4).
*/

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define MAX_STR_LEN 1024

/* Search for pattern if file. */
int main(int argc, char* argv[]) {
    char **v;
    char *sp, *sp2, *pt;
    char c, s[MAX_STR_LEN];
    bool except, number;
    long lineno;
    FILE *fp;

    /* read options */
    except = false;
    number = false;
    v = argv + 1;
    for (; *v && **v == '-'; ++v)
        while ((c = *++*v))
            switch (c) {
            case 'x':
                except = true;
                break;
            case 'n':
                number = true;
                break;
            default:
                fprintf(stderr, "Illegal option '%c'\n", c);
                v = argv + argc - 1;
                break;
            }

    /* at this point v points to first non-option argument, which
       should be PATTERN */
    if (argc - (v - argv) != 2) {
        printf("Usage: grep [OPTIONS] PATTERN FILE\n"
               "Search for PATTERN in FILE.\n\n"
               "OPTIONS\n"
               "    -n      Show line numbers\n"
               "    -x      Exclude lines with pattern\n");
        return 0;
    }

    /* open file */
    fp = fopen(v[1], "r");
    if (fp == NULL) {
        fprintf(stderr, "Can't open '%s'\n", v[1]);
        return 0;
    }

    /* find pattern */
    lineno = 0;
    pt = *v;
    while (fgets(s, MAX_STR_LEN, fp)) {
        lineno++;
        sp = strstr(s, pt);

        if (number && ((!sp && except) || (sp && !except)))
            printf("\x1b[32m" "%ld:" "\x1b[0m", lineno);

        if (!sp && except)
            printf("%s", s);

        else if (sp && !except) {
            sp2 = s;
            do {
                for (; sp2 < sp; ++sp2)  /* string before pattern */
                    putchar(*sp2);
                sp2 += strlen(pt);
                printf("\x1b[91m" "%s" "\x1b[0m", pt);  /* colored pattern */
            } while ((sp = strstr(sp + strlen(pt), pt)));
            if (*sp2)
                printf("%s", sp2);  /* the rest of the string */
        }
    }
    fclose(fp);
    return 0;
}
