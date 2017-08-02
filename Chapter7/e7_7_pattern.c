/*
Exercise 7-7.

Modify the pattern finding program of Chapter 5 to take its input from a set
of named files or, if no files are named as arguments, from the standard input.
Should the file name be printed when a matching line is found?
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STR_MAXLEN 1024

const char *prog;  /* program name for errors */

/* open file */
void file_open(FILE **fp, const char *fname) {
    if ((*fp = fopen(fname, "r")) == NULL) {
        fprintf(stderr, "%s: can't open '%s'\n", prog, fname);
        exit(EXIT_FAILURE);
    }
}

/* main */
int main(int argc, const char *argv[]) {
    const char **v, *pat;
    char       s[STR_MAXLEN], c, *sp, *sp2;
    int        except, number, name;
    long       line;
    FILE       *fp;

    prog = *argv;
    except = number = name = 0;
    
    /* options */
    for (v = argv + 1; *v && (*v)[0] == '-'; ++v)
        while ((c = *++v[0]))
            switch (c) {
            case 'x':
                except = 1;
                break;
            case 'n':
                number = 1;
                break;
            default:
                fprintf(stderr, "%s: illegal option '%c'\n", prog, c);
                exit(EXIT_FAILURE);
            }

    /* usage */
    if (argc - (v - argv) < 2) {
        printf("Usage: pattern [OPTIONS] PATTERN FILE1 FILE2 ...\n"
               "Search for pattern in files.\n\n"
               "Options:\n"
               "    -n      Show line numbers\n"
               "    -x      Exclude lines with pattern\n");
        return 0;
    }

    /* print file name if more than one file */
    if (argc - (v - argv) > 2)
        name = 1;

    /* search for pattern in every line of every file */
    for (pat = *v++; *v; v++) {
        file_open(&fp, *v);
        
        for (line = 1; fgets(s, STR_MAXLEN, fp); line++) {
            sp = strstr(s, pat);  /* pointer to first occurence of pattern */

            /* file name and line number */
            if ((!sp && except) || (sp && !except)) {
                if (name)
                    printf("\x1b[35m" "%s" "\x1b[36m" ":" "\x1b[0m", *v);
                if (number)
                    printf("\x1b[32m" "%ld" "\x1b[36m" ":" "\x1b[0m", line);
            }

            /* line w/o pattern */
            if (!sp && except)
                printf("%s", s);

            /* line with pattern */
            else if (sp && !except) {
                sp2 = s;
                do {
                    for (; sp2 < sp; ++sp2)  /* string before pattern */
                        putchar(*sp2);
                    sp2 += strlen(pat);
                    printf("\x1b[91m" "%s" "\x1b[0m", pat);  /* pattern (highlighted) */
                } while ((sp = strstr(sp + strlen(pat), pat)));
                if (*sp2)
                    printf("%s", sp2);  /* the rest of the string */
            }
        }
    
        fclose(fp);
    }
}
