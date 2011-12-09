#define _POSIX_SOURCE /* for strtok_r */
#define _SVID_SOURCE /* for strdup */

#include <glob.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "findlibrary.h"

char **ldLibraryPath = NULL;

char *globLibrary(char *buf, const char *dir, const char *base);

char *findLibrary(const char *metanm)
{
    char *metanmd, *postlib, *filenm, *foundlib;
    char *ldLibraryPathEnv, *saveptr;
    int i, llpElems, maxLen = 16;

    /* get our LD_LIBRARY_PATH if applicable */
    if (ldLibraryPath == NULL) {
        ldLibraryPathEnv = getenv("LD_LIBRARY_PATH");
        if (ldLibraryPathEnv) {
            /* figure out how many entries */
            ldLibraryPathEnv = strdup(ldLibraryPathEnv);
            if (ldLibraryPathEnv == NULL) {
                perror("strdup");
                exit(1);
            }
            llpElems = 1;
            for (i = 0; ldLibraryPathEnv[i]; i++) {
                if (ldLibraryPathEnv[i] == ':') llpElems++;
            }

            /* split it up */
            ldLibraryPath = malloc((llpElems + 1) * sizeof(char*));
            if (ldLibraryPathEnv == NULL) {
                perror("malloc");
                exit(1);
            }
            ldLibraryPath[0] = strtok_r(ldLibraryPathEnv, ":", &saveptr);
            if (strlen(ldLibraryPath[0]) > maxLen) maxLen = strlen(ldLibraryPath[0]);
            for (i = 1; (ldLibraryPath[i] = strtok_r(NULL, ":", &saveptr)); i++)
                if (strlen(ldLibraryPath[i]) > maxLen) maxLen = strlen(ldLibraryPath[i]);

        } else {
            /* empty list */
            ldLibraryPath = malloc(sizeof(char*));
            if (ldLibraryPath == NULL) {
                perror("malloc");
                exit(1);
            }
            ldLibraryPath[0] = NULL;

        }
    }

    maxLen += 32;

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
    filenm = malloc(strlen(postlib) + maxLen);
    if (filenm == NULL) {
        perror("malloc");
        exit(1);
    }

    /* 'core' libs are hardwired */
    if (!strncmp(metanm, "ld", 2) ||
        !strncmp(metanm, "libc.so.", 8) ||
        !strncmp(metanm, "libm.so.", 8) ||
        !strncmp(metanm, "libintl.so.", 11) ||
        !strncmp(metanm, "libiconv.so.", 12) ||
        !strncmp(metanm, "libsocket.so.", 13) ||
        !strncmp(metanm, "libgen.so.", 10) ||
        !strncmp(metanm, "librt.so.", 9)) {
        foundlib = findLibrary("libgelfload-host-core.so");
    } else {

        /* split it by dots */
        while (1) {
            /* first try LD_LIBRARY_PATH */
            for (i = 0; ldLibraryPath[i]; i++) {
                if ((foundlib = globLibrary(filenm, ldLibraryPath[i], postlib))) break;
            }
            if (ldLibraryPath[i]) break;
    
            /* then try /lib and /usr/lib */
            if ((foundlib = globLibrary(filenm, "/lib", postlib))) break;
            if ((foundlib = globLibrary(filenm, "/usr/lib", postlib))) break;
    
            /* then with 32 or 64 */
            if (sizeof(size_t) == 8) {
                if ((foundlib = globLibrary(filenm, "/lib64", postlib))) break;
                if ((foundlib = globLibrary(filenm, "/usr/lib64", postlib))) break;
            } else if (sizeof(size_t) == 4) {
                if ((foundlib = globLibrary(filenm, "/lib32", postlib))) break;
                if ((foundlib = globLibrary(filenm, "/usr/lib32", postlib))) break;
            }
    
            /* of course that list isn't very portable */
    
            /* now remove a bit */
            foundlib = strrchr(postlib, '.');
            if (foundlib == NULL) break;
            *foundlib = '\0';
        }

    }

    free(metanmd);
    if (foundlib) fprintf(stderr, "Found %s for %s\n", foundlib, metanm);
    return foundlib;
}


/* glob this particular library name */
char *globLibrary(char *buf, const char *dir, const char *base)
{
    char *globstr;
    glob_t pglob;
    int globr;

    sprintf(buf, "%s/lib%s.*", dir, base);

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
