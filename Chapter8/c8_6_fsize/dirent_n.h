/* The Dirent structure contains the inode number and the name. The maximum length of
a filename component is NAME_MAX, which is a system-dependent value. opendir() returns
a pointer to a structure called DIR, analogous to FILE, which is used by readdir() and
closedir(). */

#ifndef DIRENT_N
#define DIRENT_N

#ifndef NAME_MAX
#define NAME_MAX 255  /* longest filename component; system-dependent */
#endif

typedef struct {                /* portable directory entry: */
    long ino;                   /* inode number */
    char name[NAME_MAX+1];      /* name + '\0' terminator */
} dirent_t;

typedef struct {                /* minimal DIR: no buffering, etc. */
    int      fd;                /* file descriptor for directory */
    dirent_t d;                 /* the directory entry */
} DIR_t;

DIR_t    *opendir_n(char*);
dirent_t *readdir_n(DIR_t*);
void     closedir_n(DIR_t*);

#endif