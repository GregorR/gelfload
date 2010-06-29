#include <stdlib.h>

#include "elfload.h"
#include "elfload_microcosm.h"

/* syscall functions proper are NOT MICROCOSM_CALLING_CONVENTION functions */
typedef size_t (*microcosm_syscall_t)(size_t, size_t, size_t, size_t);

MICROCOSM_CALLING_CONVENTION(size_t microcosm_syscall(size_t sc, size_t a, size_t b, size_t c, size_t d));
MICROCOSM_CALLING_CONVENTION(size_t microcosm_register_syscall(size_t sc, microcosm_syscall_t func, microcosm_syscall_t *prevfunc));

void *elfload_microcosm(const char *fname)
{
    if (strcmp(fname, "microcosm_syscall") == 0) {
        return (void *) microcosm_syscall;
    } else if (strcmp(fname, "microcosm_register_syscall") == 0) {
        return (void *) microcosm_register_syscall;
    }
}

/* the microcosm syscalls */
#define MICROCOSM_SYSCALL_MAX 2048
static microcosm_syscall_t microcosm_syscalls[MICROCOSM_SYSCALL_MAX];
static size_t microcosm_syscall_count = 0;

/* call a syscall */
size_t microcosm_syscall(size_t sc, size_t a, size_t b, size_t c, size_t d)
{
    microcosm_syscall_t syscall;

    if (sc >= microcosm_syscall_count) {
        return (size_t) -1; /* FIXME: ENOTSUP */
    }

    syscall = microcosm_syscalls[sc];
    if (syscall == NULL) {
        return (size_t) -1; /* FIXME: ENOTSUP */
    }

    return syscall(a, b, c, d);
}

/* register a syscall */
size_t microcosm_register_syscall(size_t sc, microcosm_syscall_t func, microcosm_syscall_t *prevfunc)
{
    if (sc >= MICROCOSM_SYSCALL_MAX) {
        return (size_t) -1; /* FIXME: not sure what error */
    }

    /* make room */
    while (sc >= microcosm_syscall_count) {
        microcosm_syscalls[microcosm_syscall_count++] = NULL;
    }

    /* save the old one */
    if (prevfunc) {
        *prevfunc = microcosm_syscalls[sc];
    }

    /* assign */
    microcosm_syscalls[sc] = func;

    return 0;
}
