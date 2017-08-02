#include "fileio.h"
#include <stdarg.h>
#include <stdlib.h>


#define PERMS 0644  /* RW for owner, R for group and others */

FILE _iob[OPEN_MAX] = {     /* stdin, stdout, stderr */
    { 0, (char*) 0, (char*) 0, 0,  1, 0, 0, 0, 0 },
    { 0, (char*) 0, (char*) 0, 1,  0, 1, 0, 0, 0 },
    { 0, (char*) 0, (char*) 0, 2,  0, 1, 1, 0, 0 }
};

/* Open file, return file ptr. */
FILE *fopen(char *name, char *mode) {
    int fd;
    FILE *fp;

    if (*mode != 'r' && *mode != 'w' && *mode != 'a')
        return NULL;

    for (fp = _iob; fp < _iob + OPEN_MAX; fp++)
        if (!(fp->_READ || fp->_WRITE))
            break;      /* found free slot */
    
    if (fp >= _iob + OPEN_MAX)  /* no free slots */
        return NULL;

    if (*mode == 'w')
        fd = creat(name, PERMS);
    else if (*mode == 'a') {
        if ((fd = open(name, O_WRONLY)) == -1)
            fd = creat(name, PERMS);
        else
            lseek(fd, 0L, SEEK_END);
    } else
        fd = open(name, O_RDONLY);
    
    if (fd == -1)       /* couldn't acces file name */
        return NULL;

    fp->fd = fd;
    fp->cnt = 0;
    fp->base = NULL;
    if (*mode == 'r')
        fp->_READ = 1;
    else
        fp->_WRITE = 1;

    return fp;
}

/* Close file, flush i/o buffer. */
int fclose(FILE *fp) {
    
    /* flush buffer */
    if (fp->_WRITE)
        fflush(fp);

    fp->cnt = 0;
    fp->ptr = NULL;

    /* deallocate buffer */
    if (fp->base) {
        free(fp->base);
        fp->base = NULL;
    }

    /* close file */
    if (close(fp->fd) == EOF) {
        fp->_ERR = 1;
        return EOF;
    }

    fp->_READ = 0;
    fp->_WRITE = 0;
    fp->_UNBUF = 0;
    fp->_EOF = 0;
    fp->_ERR = 0;

    return 0;
}

/* Flush output buffer of a file. */
int fflush(FILE *fp) {
    int bufsize, buflen;

    if (_fpinvalid(fp) || fp->_EOF)
        return EOF;

    /* buffer is not initialized */
    if (fp->base == NULL)
        return 0;

    /* flush buffer */
    bufsize = fp->_UNBUF ? 1 : BUFSIZ;
    buflen = bufsize - (fp->cnt > 0 ? fp->cnt : 0);

    if (fp->_WRITE && buflen)
        if (write(fp->fd, fp->base, buflen) != buflen) {
            fp->_ERR = 1;
            return EOF;
        }

    fp->cnt = fp->_WRITE ? bufsize : 0;
    fp->ptr = fp->base;

    return 0;
}

/* Set the file position to given offset. */
int fseek(FILE *fp, long offset, int origin) {
    int offset_abs;

    /* flush buffer */
    if (fflush(fp) == EOF)
        return EOF;

    /* seek */
    if ((offset_abs = lseek(fp->fd, offset, origin)) == EOF) {
        fp->_ERR = 1;
        return EOF;
    }

    return offset_abs;
}

/* Allocate and fill input buffer. */
int _fillbuf(FILE *fp) {
    int bufsize;

    if (_fpinvalid(fp) || !fp->_READ || fp->_EOF)
        return EOF;

    bufsize = fp->_UNBUF ? 1 : BUFSIZ;
    if (fp->base == NULL)       /* no buffer yet */
        if ((fp->base = malloc(bufsize)) == NULL)
            return EOF;         /* can't get buffer */

    fp->ptr = fp->base;
    fp->cnt = read(fp->fd, fp->base, bufsize);
    if (--fp->cnt < 0) {
        if (fp->cnt == -1)
            fp->_EOF = 1;
        else
            fp->_ERR = 1;
        fp->cnt = 0;
        return EOF;
    }

    return (unsigned char) *fp->ptr++;
}

/* Flush output buffer and add input character. */
int _flushbuf(int c, FILE *fp) {
    int bufsize;

    if (_fpinvalid(fp) || !fp->_WRITE || fp->_EOF)
        return EOF;

    /* flush buffer */
    bufsize = fp->_UNBUF ? 1 : BUFSIZ;
    if (fp->base == NULL) {     /* no buffer yet */
        if ((fp->base = malloc(BUFSIZ)) == NULL)
            return EOF;         /* can't get buffer */
    } else
        if (write(fp->fd, fp->base, bufsize) != bufsize) {
            fp->_ERR = 1;
            return EOF;
        }

    fp->cnt = bufsize;
    fp->ptr = fp->base;

    /* add character */
    *fp->ptr++ = c;
    fp->cnt--;

    return 0;
}

/* Validate file pointer. */
int _fpinvalid(FILE *fp) {
    if (!fp ||                          /* pointer is not NULL */
        !(fp->_READ || fp->_WRITE) ||   /* file is opened */
        fp->_ERR ||                     /* no error flag */
        fcntl(fp->fd, F_GETFD))         /* file descriptor is valid */
        return 1;
    return 0;
}
