#include <stdio.h>
#include <unistd.h>

/* Read n bytes from position pos. */
int get(int fd, size_t pos, char *buf, int n) {
    if (lseek(fd, pos, SEEK_SET) >= 0)  /* get to pos */
        return read(fd, buf, n);
    else
        return -1;
}
