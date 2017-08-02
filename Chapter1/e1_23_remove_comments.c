/*
Exercise 1-23.

Write a program to remove all comments from a C program. Don't forget to
handle quoted strings and character constants properly. C comments do not
nest.
*/

#include <stdio.h>
#include <stdbool.h>


/* ---------------------------------------------------------------------------- */
/* Input buffer */

#define IN_SIZE 1024

char   in_buf[IN_SIZE];
size_t in_len;
size_t in_idx;


/* Fill buffer from stdin. */
size_t in_read() {
    int c;

    in_idx = in_len = 0;
    for (in_len; in_len < IN_SIZE - 1 && (c = getchar()) != EOF; in_len++)
        in_buf[in_len] = c;
    in_buf[in_len] = '\0';
    return in_len;
}


/* Pop character from buffer. */
int in_pop() {
    if (in_idx == in_len)
        in_read();
    return in_len == 0 ? 0 : in_buf[in_idx++];
}


/* ---------------------------------------------------------------------------- */
/* Output buffer */

#define OUT_SIZE 1024

char   out_buf[OUT_SIZE];
size_t out_idx;


/* Flush buffer to stdout. */
void out_flush() {
    out_buf[out_idx] = '\0';
    out_idx = 0;
    printf("%s", out_buf);
}


/* Add character to buffer; flush if necessary. */
void out_add(int c) {
    if (c == '\0')  /* ignore leading zeros in characters stream */
        return;
    if (out_idx == OUT_SIZE - 1)
        out_flush();
    out_buf[out_idx++] = c;
}


/* ---------------------------------------------------------------------------- */
/* Characters stream */

#define CH_SIZE 3

char ch_buf[CH_SIZE];


/* Add character from input buffer to stream; return that character. */
int ch_get() {
    int i;

    for (i = 0; i < CH_SIZE - 1; i++)
        ch_buf[i] = ch_buf[i+1];    /* shift values to the left */
    return ch_buf[i] = in_pop();    /* get character */
}


/* ---------------------------------------------------------------------------- */
/* Main */

/* Tests. */
#define comment_beg  (ch_buf[1] == '/' && ch_buf[2] == '*')
#define comment_end  (ch_buf[1] == '*' && ch_buf[2] == '/')
#define squotes_beg  (ch_buf[2] == '\'')
#define squotes_end  ((ch_buf[0] == '\\' || ch_buf[1] != '\\') && ch_buf[2] == '\'')
#define dquotes_beg  (ch_buf[2] == '\"')
#define dquotes_end  ((ch_buf[0] == '\\' || ch_buf[1] != '\\') && ch_buf[2] == '\"')

enum Status { NONE, READ, COMMENT, SQUOTES, DQUOTES };


int main() {
    bool do_read;
    enum Status status;

    status = READ;
    do_read = in_read();

    /* read/write cycle */    
    while (do_read) {

        if (ch_get() == 0) {
            do_read = false;
            status = NONE;
        }

        switch (status) {

        case READ:
            if (comment_beg) {
                status = COMMENT;
                out_add(' ');  /* replace comment with blank */
            } else if (dquotes_beg) {
                status = DQUOTES;
                out_add(ch_buf[1]);
            } else if (squotes_beg) {
                status = SQUOTES;
                out_add(ch_buf[1]);
            } else
                out_add(ch_buf[1]);
            break;

        case COMMENT:
            while (status == COMMENT && do_read) {
                if (comment_end)
                    status = READ;
                do_read = ch_get();
            }
            break;

        case SQUOTES:
            if (squotes_end)
                status = READ;
            out_add(ch_buf[1]);
            break;

        case DQUOTES:
            if (dquotes_end)
                status = READ;
            out_add(ch_buf[1]);
            break;

        default:
            out_add(ch_buf[1]);
            break;
        }
    }

    out_flush();

    return 0;
}
