/* microcosm interface */

#ifndef ELFLOAD_MICROCOSM_H
#define ELFLOAD_MICROCOSM_H

#if (defined(__i386__) || defined(__i386) || defined(_M_IX86) || defined(_X86_)) && \
    !(defined(__x86_64__) || defined(_M_X64))
#define MICROCOSM_CALLING_CONVENTION(f) f __attribute__((cdecl))
#elif defined(__x86_64__) || defined(_M_X64)
#define MICROCOSM_CALLING_CONVENTION(f) f __attribute__((sysv_abi))
#else
#define MICROCOSM_CALLING_CONVENTION(f) f
#endif

/* return a microcosm function by its name */
void *elfload_microcosm(const char *fname);

#endif
