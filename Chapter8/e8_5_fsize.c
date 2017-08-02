/*
Exercise 8-5.

Modify the fsize program to print the other information contained in the inode
entry.
*/

#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#define MAX_PATH    1024
#define btoK(x)     (x / 1024 + (((long)(x % 1024) - 512) > 0 ? 1 : 0))
#define btoM(x)     (x / 1048576 + (((long)(x % 1048576) - 524288) > 0 ? 1 : 0))
#define btoG(x)     (x / 1073741824 + (((long)(x % 1073741824) - 536870912) > 0 ? 1 : 0))

static int    total_ent;
static size_t total_siz;

/* Convert size to human-readable string */
static char *lssize(size_t size) {
    static char siz[5];

    if (size < 1000u)
        sprintf(siz, "%4lu", size);
    else if (size < 1024000u)
        sprintf(siz, "%3luK", btoK(size));
    else if (size < 1048576000u)
        sprintf(siz, "%3luM", btoM(size));
    else
        sprintf(siz, "%3luG", btoG(size));

    return siz;
}

/* Convert a mode field into "ls -l" type perms field. */
static char *lsperms(int mode) {
    static const char *rwx[] = { "---", "--x", "-w-", "-wx", "r--", "r-x", "rw-", "rwx" };
    static char bits[11];

    /* file type letter */
    switch (mode & S_IFMT) {
    case S_IFBLK:  bits[0] = 'b'; break;
    case S_IFCHR:  bits[0] = 'c'; break;
    case S_IFDIR:  bits[0] = 'd'; break;
    case S_IFIFO:  bits[0] = 'p'; break;
    case S_IFLNK:  bits[0] = 'l'; break;
    case S_IFREG:  bits[0] = '-'; break;
    case S_IFSOCK: bits[0] = 's'; break;
    default:       bits[0] = '?'; break;
    }

    strcpy(bits + 1, rwx[(mode >> 6) & 7]);         /* user permissions */
    strcpy(bits + 4, rwx[(mode >> 3) & 7]);         /* group permissions */
    strcpy(bits + 7, rwx[mode & 7]);                /* others permissions*/
    if (mode & S_ISUID)
        bits[3] = (mode & S_IXUSR) ? 's' : 'S';     /* SUID */
    if (mode & S_ISGID)
        bits[6] = (mode & S_IXGRP) ? 's' : 'l';     /* SGID */
    if (mode & S_ISVTX)
        bits[9] = (mode & S_IXOTH) ? 't' : 'T';     /* sticky bit */
    bits[10] = '\0';

    return bits;
}

/* Convert time struct to human-readable string. */
static char *lsdate(struct timespec *tm) {
    static char datestr[256];
    struct tm   *t;

    t = localtime((const time_t*)tm);
    strftime(datestr, sizeof(datestr), "%H:%M %x", t);

    return datestr;
}

/* Apply fcn to all files in dir */
void dirwalk(char *dir, void (*fcn)(char*)) {
    char          name[MAX_PATH];
    DIR           *dirp;
    struct dirent *dp;

    if ((dirp = opendir(dir)) == NULL) {  /* open directory */
        fprintf(stderr, "dirwalk: Cannot open '%s'\n", dir);
        return;
    }

    while ((dp = readdir(dirp))) {  /* read entries */
        if (!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, ".."))
            continue;   /* skip self and parent */
        if (strlen(dir) + strlen(dp->d_name) + 2 > sizeof(name))
            fprintf(stderr, "dirwalk: Name too long\n");
        else {
            sprintf(name, "%s/%s", dir, dp->d_name);
            (*fcn)(name);
        }
    }

    closedir(dirp);
}

/* Print attributes of file "name". */
void fsize(char *name) {
    struct stat stbuf;

    if (stat(name, &stbuf) == -1) {
        fprintf(stderr, "fsize: Cannot open '%s'\n", name);
        return;
    }

    if (S_ISDIR(stbuf.st_mode))
        dirwalk(name, fsize);

    printf("%10s  %s  %s  %s\n", lsperms(stbuf.st_mode), lssize(stbuf.st_size), lsdate(&stbuf.st_mtim), name);
    total_ent++;
    total_siz += stbuf.st_size;
}

/* Print file list. */
int main(int argc, char *argv[]) {
    if (argc == 1)      /* default: current directory */
        fsize(".");
    else
        while (--argc)
            fsize(*++argv);

    printf("\n%10d entries\n", total_ent);
    printf("%10s total\n", lssize(total_siz));

    return 0;
}

