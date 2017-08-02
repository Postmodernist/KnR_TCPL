/* cat:  concatenate files, version 1 */

#include <stdio.h>

int main(int argc, char const *argv[]) {
    FILE *fp;
    void filecopy(FILE *, FILE *);

    if (argc == 1)
        filecopy(stdin, stdout);
    else
        while (--argc > 0)
            if ((fp = fopen(*++argv, "r")) == NULL) {
                printf("\x1b[91m" "error:" "\x1b[0m" " cat: can't open '%s'\n", *argv);
                return 1;
            } else {
                filecopy(fp, stdout);
                fclose(fp);
            }

    return 0;
}

/* copy file ifp to file ofp */
void filecopy(FILE *ifp, FILE *ofp) {
    int c;

    while ((c = getc(ifp)) != EOF)
        putc(c, ofp);
}
