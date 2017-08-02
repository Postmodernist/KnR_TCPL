#include <stdio.h>
#include <unistd.h>

/* Unbuffered single character input. */
int getchar_u(void) {
    char c;

    return (read(0, &c, 1) == 1) ? (unsigned char) c : EOF;
}

/* Buffered version. */
int getchar_b(void) {
    static char buf[BUFSIZ];
    static char *bufp = buf;
    static int n = 0;

    if (n == 0) {  /* buffer is empty */
        n = read(0, buf, sizeof(buf));
        bufp = buf;
    }
    return (--n >= 0) ? (unsigned char) *bufp++ : EOF;
}

int main(void) {
}
