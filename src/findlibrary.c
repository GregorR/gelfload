#include <glob.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "findlibrary.h"

char *globLibrary(char *buf, const char *dir, const char *base);

char *findLibrary(const char *metanm)
{
    char *metanmd, *postlib, *filenm, *foundlib;

    metanmd = strdup(metanm);
    if (metanmd == NULL) {
        perror("strdup");
        exit(1);
    }

    /* cut off lib */
    if (strncmp(metanmd, "lib", 3) == 0) {
        postlib = metanmd + 3;
    } else {
        postlib = metanmd;
    }

    /* make some space for it */
    filenm = malloc(strlen(postlib) + 32);
    if (filenm == NULL) {
        perror("malloc");
        exit(1);
    }

    /* split it by dots */
    while (1) {
        /* first try /lib and /usr/lib */
        if (foundlib = globLibrary(filenm, "/lib/", postlib)) break;
        if (foundlib = globLibrary(filenm, "/usr/lib/", postlib)) break;

        /* then with 32 or 64 */
        if (sizeof(size_t) == 8) {
            if (foundlib = globLibrary(filenm, "/lib64/", postlib)) break;
            if (foundlib = globLibrary(filenm, "/usr/lib64/", postlib)) break;
        } else if (sizeof(size_t) == 4) {
            if (foundlib = globLibrary(filenm, "/lib32/", postlib)) break;
            if (foundlib = globLibrary(filenm, "/usr/lib32/", postlib)) break;
        }

        /* of course that list isn't very portable */

        /* now remove a bit */
        foundlib = strrchr(postlib, '.');
        if (foundlib == NULL) break;
        *foundlib = '\0';
    }

    free(metanmd);
    return foundlib;
}


/* glob this particular library name */
char *globLibrary(char *buf, const char *dir, const char *base)
{
    char *globstr;
    glob_t pglob;
    int globr;

    sprintf(buf, "%slib%s*", dir, base);

    /* now try to glob */
    globr = glob(buf, 0, NULL, &pglob);
    if (globr == 0) {
        /* found something */
        globstr = strdup(pglob.gl_pathv[0]);
        globfree(&pglob);
        if (globstr == NULL) {
            perror("strdup");
            exit(1);
        }
        return globstr;
    }

    return NULL;
}
