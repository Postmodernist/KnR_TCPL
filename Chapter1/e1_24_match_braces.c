/*
Exercise 1-24.

Write a program to check a C program for rudimentary syntax errors like
unbalanced parentheses, brackets and braces. Don't forget about quotes,
both single and double, escape sequences, and comments. (This program is
hard if you do it in full generality.)
*/

#include <stdio.h>
#include <stdbool.h>

#define BUFFER_SIZE 1000

char br_open[7]  = "({[\"\'/";
char br_close[4] = ")}]";

struct Buffer {
    char   data[BUFFER_SIZE];
    size_t ln[BUFFER_SIZE];
    size_t col[BUFFER_SIZE];
    size_t ptr;
    bool   empty;
    bool   full;
} buf, buf2;

void b_init(struct Buffer*);
void b_push(char, size_t, size_t, struct Buffer*);
void b_pop(struct Buffer*);
bool contains(char*, char);
bool match(char, char);
void unmatch_msg(char, size_t, size_t);


/* ------------------------------------------------------ */
/* Main. */
int main() {
    extern char br_open[];
    extern char br_close[];
    size_t i, j, line, column;
    int    c, c0;   /* current and previous characters */
    bool   comment_cand, comment, multiline_comment, double_quote, single_quote, special_char;
    
    b_init(&buf);   /* buffer for unmatched open brackets */
    b_init(&buf2);  /* buffer for unmatched closed brackets */
    line = 1;
    column = 0;
    c0 = 0;
    comment_cand = false;
    comment = false;
    multiline_comment = false;
    double_quote = false;
    single_quote = false;
    special_char = false;

    /* process input character by character and detect errors */
    for (i = 0; (c = getchar()) != EOF; i++) {

        column++;

        /* match comment patterns */
        if (comment_cand) {
            if (c == '/')
                comment = true;
            else if (c == '*')
                multiline_comment = true;
            else
                b_pop(&buf);
            
            comment_cand = false;

        } else if (comment) {
            if (c == '\n') {
                b_pop(&buf);
                comment = false;
            }
        
        } else if (multiline_comment) {
            if ((c0 == '*') && (c == '/')) {
                b_pop(&buf);
                c0 = 0;
                multiline_comment = false;
            } else
                c0 = c;
        }

        /* match other patters */
        if (comment || multiline_comment)           /* comment */
            ;  

        else if (double_quote || single_quote) {    /* string constant */
            if (!special_char) {
                if ((double_quote && (c == '\"')) || (single_quote && (c == '\''))) {
                    double_quote = single_quote = false;
                    b_pop(&buf);
                } else if (c == '\\') {
                    special_char = true;
                }
            } else {
                special_char = false;
            }
        
        } else if (contains(br_open, c)) {          /* opening brackets */
            b_push(c, line, column, &buf);
            if (c == '\"')
                double_quote = true;
            else if (c == '\'')
                single_quote = true;
            else if (c == '/')
                comment_cand = true;
        
        } else if (contains(br_close, c)) {         /* closing brackets */
            if (buf.empty || !match(buf.data[buf.ptr], c)) {
                b_push(c, line, column, &buf2);
            } else {
                b_pop(&buf);
            }
        }

        /* keep track of current line and column */
        if (c == '\n') {
            line++;
            column = 0;
        }
    }

    /* clear comment if it ended with EOF instead of newline */
    if (comment)
        b_pop(&buf);

    /* dump unmatched brackets in sorted order */
    if (buf2.empty) {
        if (!buf.empty) {       /* only dump buf */
            for (i = 0; i <= buf.ptr; i++)
                unmatch_msg(buf.data[i], buf.ln[i], buf.col[i]);
        } else {
            printf("No errors detected.\n");
        }
    } else if (buf.empty) {     /* only dump buf2 */
        for (j = 0; j <= buf2.ptr; j++)
            unmatch_msg(buf2.data[j], buf2.ln[j], buf2.col[j]);
    } else {                    /* merge buf and buf2 */
        i = j = 0;
        while ((i <= buf.ptr) || (j <= buf2.ptr)) {
            if (i > buf.ptr) {          /* dump the rest of buf2 */
                while (j <= buf2.ptr) {
                    unmatch_msg(buf2.data[j], buf2.ln[j], buf2.col[j]);
                    j++;
                }
            } else if (j > buf2.ptr) {  /* dump the rest of buf */
                while (i <= buf.ptr) {
                    unmatch_msg(buf.data[i], buf.ln[i], buf.col[i]);
                    i++;
                }
            } else if (buf.ln[i] < buf2.ln[j]) {
                unmatch_msg(buf.data[i], buf.ln[i], buf.col[i]);
                i++;
            } else if (buf2.ln[j] < buf.ln[i]) {
                unmatch_msg(buf2.data[j], buf2.ln[j], buf2.col[j]);
                j++;
            } else if (buf.col[i] < buf2.col[j]) {
                unmatch_msg(buf.data[i], buf.ln[i], buf.col[i]);
                i++;
            } else {
                unmatch_msg(buf2.data[j], buf2.ln[j], buf2.col[j]);
                j++;
            }
        }
    }

    return 0;
}


/* ------------------------------------------------------ */
/* Buffer methods. */

void b_init(struct Buffer *b) {
    b->ptr = 0;
    b->empty = true;
    b->full = false;
}


void b_push(char c, size_t ln, size_t col, struct Buffer *b) {
    if (b->full) {
        fprintf(stderr, "b_push: Buffer overflow, skipping '%c' at (%ld:%ld).\n", c, ln, col);
    } else {
        if (!b->empty)
            b->ptr++;
        b->data[b->ptr] = c;
        b->ln[b->ptr] = ln;
        b->col[b->ptr] = col;
        b->empty = false;
        if (b->ptr == BUFFER_SIZE - 1) {
            b->full = true;
        }
    }
}


void b_pop(struct Buffer *b) {
    if (!b->empty) {
        if (b->ptr == 0) {
            b->empty = true;
        } else {
            b->ptr--;
        }
        b->full = false;
    }
}


/* ------------------------------------------------------ */
/* Utils. */

/* Return true if str contains chr. */
bool contains(char *str, char chr) {
    for (; *str; str++)
        if (*str == chr)
            return true;
    return false;
}


/* Match parentheses, braces, and brackets. */
bool match(char a, char b) {
    extern char br_open[];
    extern char br_close[];
    int i = 0;

    while ((br_open[i] != a) && (br_open[i] != '\0'))
        i++;

    return b == br_close[i];
}


/* Print error message. */
void unmatch_msg(char c, size_t ln, size_t col) {
    if (c != '/')
        printf("Unmatched '%c' at (%ld:%ld).\n", c, ln, col);
    else
        printf("Unterminated comment at (%ld:%ld).\n", ln, col);
}
