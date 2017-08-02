/* Data structure that describes a file. */
#ifndef FILEIO_H
#define FILEIO_H

#include <fcntl.h>
#include <unistd.h>

#define EOF         (-1)
#define BUFSIZ      8
#define OPEN_MAX    4  /* max files open at once */

typedef struct _iobuf {
    int  cnt;           /* characters left */
    char *ptr;          /* next character position */
    char *base;         /* location of buffer */
    int fd;             /* file descriptor */
    int _READ  : 1;     /* file open for reading */
    int _WRITE : 1;     /* file open for writing */
    int _UNBUF : 1;     /* file is unbuffered */
    int _EOF   : 1;     /* EOF has occurred on this file */
    int _ERR   : 1;     /* error occurred on this file */
} FILE;

extern FILE _iob[OPEN_MAX];

#define stdin   (&_iob[0])
#define stdout  (&_iob[1])
#define stderr  (&_iob[2])

FILE* fopen(char*, char*);
int   fclose(FILE*);
int   fflush(FILE*);
int   fseek(FILE*, long, int);

int _fillbuf(FILE*);
int _flushbuf(int, FILE*);
int _fpinvalid(FILE*);

#define feof(p)     ((p)->_EOF)
#define ferror(p)   ((p)->_ERR)
#define fileno(p)   ((p)->fd)

#define getc(p)     (--(p)->cnt >= 0 ? (unsigned char) *(p)->ptr++ : _fillbuf(p))
#define putc(x, p)  (--(p)->cnt >= 0 ? *(p)->ptr++ = (x) : _flushbuf((x), p))
#define getchar()   getc(stdin)
#define putchar(x)  putc((x), stdout)

#endif
