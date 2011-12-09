/*
 * Copyright (C) 2005, 2006  Gregor Richards
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#define _SVID_SOURCE /* for strdup */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

/* Separate a full path into a directory and file component (internal)
 * full: Full path
 * dir: Where to put the dir component
 * fil: Where to put the file component */
void dirAndFil(const char *full, char **dir, char **fil)
{
    *dir = strdup(full);
    *fil = strrchr(*dir, '/');
    if (*fil) {
        **fil = '\0';
        (*fil)++;
        *fil = strdup(*fil);
    } else {
        *fil = (char *) malloc(1);
        if (!(*fil)) {
            perror("malloc");
            exit(1);
        }
        **fil = '\0';
    }
}

/* Figure out where a binary is installed
 * argvz: argv[0]
 * dir: Where to put the directory component
 * fil: Where to put the filename component
 * returns a pointer to dir or NULL for failure */
char *whereAmI(const char *argvz, char **dir, char **fil)
{
    char *workd, *path, *retname;
    char *pathelem[1024];
    int i, j, osl;
    struct stat sbuf;
    char *argvzd = strdup(argvz);
    if (!argvzd) { perror("strdup"); exit(1); }

#ifdef __WIN32
    for (i = 0; argvzd[i]; i++) {
        if (argvzd[i] == '\\') {
            argvzd[i] = '/';
        }
    }
#endif
    
    /* 1: full path, yippee! */
    if (argvzd[0] == '/') {
        dirAndFil(argvzd, dir, fil);
        return argvzd;
    }
    
#ifdef __WIN32
    /* 1.5: full path on Windows */
    if (argvzd[1] == ':' &&
        argvzd[2] == '/') {
        dirAndFil(argvzd, dir, fil);
        return argvzd;
    }
#endif
    
    /* 2: relative path */
    if (strchr(argvzd, '/')) {
        workd = (char *) malloc(1024 * sizeof(char));
        if (!workd) { perror("malloc"); exit(1); }
        
        if (getcwd(workd, 1024)) {
            retname = (char *) malloc((strlen(workd) + strlen(argvzd) + 2) * sizeof(char));
            if (!retname) { perror("malloc"); exit(1); }
            
            sprintf(retname, "%s/%s", workd, argvzd);
            free(workd);
            
            dirAndFil(retname, dir, fil);
            free(argvzd);
            return retname;
        }
    }
    
    /* 3: worst case: find in PATH */
    path = getenv("PATH");
    if (path == NULL) {
        return NULL;
    }
    path = strdup(path);
    
    /* tokenize by : */
    memset(pathelem, 0, 1024 * sizeof(char *));
    pathelem[0] = path;
    i = 1;
    osl = strlen(path);
    for (j = 0; j < osl; j++) {
        for (; path[j] != '\0' && path[j] != ':'; j++);
        
        if (path[j] == ':') {
            path[j] = '\0';
            
            j++;
            pathelem[i++] = path + j;
        }
    }
    
    /* go through every pathelem */
    for (i = 0; pathelem[i]; i++) {
        retname = (char *) malloc((strlen(pathelem[i]) + strlen(argvzd) + 2) * sizeof(char));
        if (!retname) { perror("malloc"); exit(1); }
        
        sprintf(retname, "%s/%s", pathelem[i], argvzd);
        
        if (stat(retname, &sbuf) == -1) {
            free(retname);
            continue;
        }
        
        if (sbuf.st_mode & S_IXUSR) {
            dirAndFil(retname, dir, fil);
            free(argvzd);
            return retname;
        }
        
        free(retname);
    }
    
    /* 4: can't find it */
    dir = NULL;
    fil = NULL;
    free(argvzd);
    return NULL;
}
