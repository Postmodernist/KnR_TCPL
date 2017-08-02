#include <dirent.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "dirent_n.h"

/* Open a directory for readdir calls. */
DIR_t *opendir_n(char *dir) {
    int fd;
    struct stat stbuf;
    DIR_t *dirp;

    if ((fd = open(dir, O_RDONLY)) == -1 ||
        fstat(fd, &stbuf) == -1 ||
        S_ISDIR(stbuf.st_mode) == 0 ||
        !(dirp = malloc(sizeof(DIR_t))))
        return NULL;
    
    dirp->fd = fd;

    return dirp;
}

/* Close directory opened by opendir. */
void closedir_n(DIR_t *dirp) {
    if (dirp) {
        close(dirp->fd);
        free(dirp);
    }
}

#define FILE_MAX 1024           /* maximum opened files*/
static DIR *dirptrs[FILE_MAX];  /* buffer for DIR object pointers */

/* Read directory structure. */
dirent_t *readdir_n(DIR_t *dirp_n) {
    struct dirent   *dp;
    static dirent_t dp_n;

    if (dirptrs[dirp_n->fd] == NULL)  /* new directory */
        dirptrs[dirp_n->fd] = fdopendir(dirp_n->fd);
    
    while ((dp = readdir(dirptrs[dirp_n->fd]))) {  /* read next entry */
        strncpy(dp_n.name, dp->d_name, NAME_MAX);
        dp_n.name[NAME_MAX] = '\0';
        return &dp_n;
    }

    closedir(dirptrs[dirp_n->fd]);  /* close DIR object */
    dirptrs[dirp_n->fd] = NULL;

    return NULL;
}
