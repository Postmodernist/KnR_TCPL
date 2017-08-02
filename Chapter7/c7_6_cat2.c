/* cat:  concatenate files, version 2 */

#include <stdio.h>
#include <stdlib.h>

static const char prog[] = "cat";  /* program name for errors */

int main(int argc, char const *argv[]) {
    FILE *fp;
    void filecopy(FILE *, FILE *);

    if (argc == 1)
        filecopy(stdin, stdout);
    else
        while (--argc > 0)
            if ((fp = fopen(*++argv, "r")) == NULL) {
                fprintf(stderr, "%s: can't open '%s'\n", prog, *argv);
                exit(EXIT_FAILURE);
            } else {
                filecopy(fp, stdout);
                fclose(fp);
            }

    if (ferror(stdout)) {
        fprintf(stderr, "%s: error writing stdout\n", prog);
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}

/* copy file ifp to file ofp */
void filecopy(FILE *ifp, FILE *ofp) {
    char buf[BUFSIZ];

    while (fgets(buf, BUFSIZ, ifp))
        if (fputs(buf, ofp) == EOF) {
            fprintf(stderr, "%s: writing failed\n", prog);
            exit(EXIT_FAILURE);
        }
}
