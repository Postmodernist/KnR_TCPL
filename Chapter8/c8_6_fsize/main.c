/* Print the size of all files named in command-line argument list. If one of the files
is a directory, fsize applies itself recursively to that directory. If there are no
arguments at all, it processes the current directory. */

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include "dirent_n.h"

#define MAX_PATH 1024

void fsize(char*);
void dirwalk(char*, void (*fcn)(char*));

/* Print file sizes. */
int main(int argc, char *argv[]) {
    if (argc == 1)      /* default: current directory */
        fsize(".");
    else
        while (--argc)
            fsize(*++argv);

    return 0;
}

/* Print size of file "name". */
void fsize(char *name) {
    struct stat stbuf;

    if (stat(name, &stbuf) == -1) {
        fprintf(stderr, "fsize: couldn't open '%s'\n", name);
        return;
    }
    
    if (S_ISDIR(stbuf.st_mode))
        dirwalk(name, fsize);

    printf("%8lu %s\n", stbuf.st_size, name);
}

/* Apply fcn to all files in dir */
void dirwalk(char *dir, void (*fcn)(char*)) {
    char     name[MAX_PATH];
    dirent_t *dp;
    DIR_t    *dirp;

    if (!(dirp = opendir_n(dir))) {
        fprintf(stderr, "dirwalk: couldn't open '%s'\n", dir);
        return;
    }
    
    while ((dp = readdir_n(dirp))) {
        if (!strcmp(dp->name, ".") || !strcmp(dp->name, ".."))
            continue;   /* skip self and parent */
        if (strlen(dir) + strlen(dp->name) + 2 > sizeof(name))
            fprintf(stderr, "dirwalk: name '%s/%s' is too long\n", dir, dp->name);
        else {
            sprintf(name, "%s/%s", dir, dp->name);
            (*fcn)(name);
        }
    }
    
    closedir_n(dirp);
}
