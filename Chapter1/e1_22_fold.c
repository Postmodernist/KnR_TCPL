/*
Exercise 1-22.

Write a program to "fold" long input lines into two or more shorter lines after
the last non-blank character that occurs before the n-th columnt of input. Make
sure your program does something intelligent with very long lines, and if there
are no blanks or tabs before the specified column.
*/

#include <stdio.h>
#include <stdbool.h>

#define MAXLINE     1000    /* line buffer size */
#define FOLD_LEN    80      /* fold threshold */
#define TABSTOP     4       /* tab stop interval */
#define NEXT_TAB(x) (TABSTOP - (x) % TABSTOP)


/* Return true if c is blank. */
bool isblank_n(char c) {
    return c == ' ' || c == '\t';
}


/* Read a line from stdin into s, return length. */
size_t lineread(char *s, size_t lim) {
    int c;
    size_t i;

    for (i = 0; (i < lim - 1) && ((c = getchar()) != EOF) && (c != '\n'); ++i)
        s[i] = c;
    if (c == '\n')
        s[i++] = c;
    s[i] = '\0';
    return i;
}


/* Copy n chars starting from beg from src into dst; assume dst is big enough. */
size_t linecopyrng(char *dst, char *src, size_t beg, size_t n) {
    size_t i;
    
    for (i = 0; i < n && (dst[i] = src[i + beg]) != '\0'; i++)
        ;
    if (i == n)
        dst[i] = '\0';
    return i;
}


int main() {
    size_t len;             /* line length */
    size_t ulen;            /* length of tail (last chunk w/o '\n') */
    long   rest;            /* columns left before folding */
    size_t beg;             /* beginning of line chunk */
    size_t end;             /* end of line chunk */
    size_t end_bak;         /* backup of end */
    size_t col;             /* colum index for tabs expansion */
    size_t i, tmp;
    char line[MAXLINE];     /* line buffer */
    char buf[FOLD_LEN+1];   /* line chunk buffer */
    bool folding;

    ulen = 0;
    while ((len = lineread(line, MAXLINE)) > 0) {
        /* print short line */
        if (ulen + len < FOLD_LEN) {
            /* save tail length... */
            ulen = line[len-1] == '\n' ? 0 : ulen + len;
            /* ... and print it */
            printf("%s", line);
            continue;
        }

        /* fold line */
        folding = true;
        beg = end = 0;
        
        while (folding) {
            
            /* expand tabs */
            /* i        line buffer index */
            /* col      current column */
            /* rest     columns left before fold */
            rest = FOLD_LEN - ulen - 1;
            for (i = beg, col = ulen; i < len && rest > 0; i++) {
                if (line[i] == '\t') {
                    tmp = NEXT_TAB(col);
                    col += tmp;
                    rest -= tmp;
                } else {
                    col++;
                    rest--;
                }
            }
            end = end_bak = i;
            
            /* scan backwards for blank */
            for (; end != beg && !isblank_n(line[end]); end--)
                ;
            
            if (end != beg) {       /* blank found */
                end_bak = end;
                /* scan for non-blank */
                for (; end != beg && isblank_n(line[end]); end--)
                    ;
                if (end == beg)     /* line is blank up to fold point */
                    end = end_bak;  /* backtrack to first found blank */
                else
                    end++;
            
            } else {                /* no blanks, just wrap the line */
                end = end_bak;
            }
            
            /* now end is at the folding point */
            linecopyrng(buf, line, beg, end - beg);
            printf("%s\n", buf);
            beg = end;  /* end of chunk is a beginning of next chunk */
            ulen = 0;   /* reset tail length */

            if (len - beg < FOLD_LEN) {  /* tail is short enough */
                /* save tail length */
                ulen = line[len-1] == '\n' ? 0 : len - beg;
                linecopyrng(buf, line, beg, len - beg);
                printf("%s", buf);
                folding = false;
            }
        }
    }

    return 0;
}
