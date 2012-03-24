#ifndef _GNU_SOURCE /* for RTLD_DEFAULT */
#define _GNU_SOURCE 1
#endif

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "bbuffer.h"

#include "../config.h"

#ifdef HAVE_MMAP
#include <sys/mman.h>
#endif

#ifdef HAVE_DLFCN_H
#include <dlfcn.h>
#endif

#ifdef __WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#endif

#include "elfload.h"
#include "elfload_dlfcn.h"

/* An array of files currently in the process of loading */
#define MAX_ELF_FILES 255
struct ELF_File elfFiles[MAX_ELF_FILES];
int elfFileCount = 0;

/* The function to actually load ELF files into memory */
struct ELF_File *loadELF(const char *nm, const char *instdir)
{
    int i, fileNo, phdri;
    struct ELF_File *f;
    void *curphdrl;
    ElfNative_Phdr *curphdr;
    ElfNative_Dyn *curdyn;

    /* first, make sure it's not already loaded or loading */
    for (i = 0; i < elfFileCount; i++) {
        if (strcmp(elfFiles[i].nm, nm) == 0) return &(elfFiles[i]);
    }

    /* now start preparing to load it */
    fileNo = elfFileCount;
    f = &(elfFiles[fileNo]);
    memset(f, 0, sizeof(struct ELF_File));
    elfFileCount++;
    f->nm = strdup(nm);

    /* if this is a host library, circumvent all the ELF stuff and go straight for the host */
    if (strncmp(nm, "libhost_", 8) == 0) {
        f->hostlib = HOSTLIB_HOST;
#if defined(HAVE_DLFCN_H)
        if (strcmp(nm, "libhost_.so") == 0) {
            /* the entire host */
#ifdef RTLD_DEFAULT
            f->prog = RTLD_DEFAULT;
#else
            f->prog = dlopen(NULL, RTLD_NOW|RTLD_GLOBAL);
#endif
        } else {
            f->prog = dlopen(nm + 8, RTLD_NOW|RTLD_GLOBAL);

            if (f->prog == NULL) {
                fprintf(stderr, "Could not resolve host library %s.\n", nm + 8);
                exit(1);
            }
        }
#elif defined(__WIN32)
        if (strcmp(nm, "libhost_.so") == 0) {
            f->prog = LoadLibrary("msvcrt.dll");
        } else {
            f->prog = LoadLibrary(nm + 8);
        }
        if (f->prog == NULL) {
            fprintf(stderr, "Could not resolve host library %s.\n", nm + 8);
            exit(1);
        }
#else
        fprintf(stderr, "This version of elfload is not capable of loading the host library %s.\n",
                nm + 8);
        exit(1);
#endif
        return f;

    } else if (strncmp(nm, "libloader_", 10) == 0) {
        /* must be provided by the loader. Only dl.0 is provided right now */
        if (strcmp(nm, "libloader_dl.0") == 0) {
            f->hostlib = HOSTLIB_DL;

        } else {
            fprintf(stderr, "Loader lib %s unsupported.\n", nm);
            exit(1);

        }

        return f;
    }

    readFile(nm, instdir, f);

    /* make sure it's an ELF file */
    f->ehdr = (ElfNative_Ehdr *) f->prog;
    if (memcmp(f->ehdr->e_ident, ELFMAG, SELFMAG) != 0) {
        fprintf(stderr, "%s does not appear to be an ELF file.\n", nm);
        exit(1);
    }

    /* only native-bit supported for the moment */
    if ((SIZEOF_VOID_P == 4 && f->ehdr->e_ident[EI_CLASS] != ELFCLASS32) ||
        (SIZEOF_VOID_P == 8 && f->ehdr->e_ident[EI_CLASS] != ELFCLASS64)) {
        fprintf(stderr, "%s is not a %d-bit ELF file.\n", nm, SIZEOF_VOID_P * 8);
        exit(1);
    }

    /* FIXME: check endianness */

    /* must be an executable or .so to be loaded */
    if (f->ehdr->e_type != ET_EXEC &&
        f->ehdr->e_type != ET_DYN) {
        fprintf(stderr, "%s is not an executable or shared object file.\n", nm);
        exit(1);
    }

    /* now go through program headers, to find the allocation space of this file */
    f->min = (void *) -1;
    f->max = 0;
    curphdrl = f->prog + f->ehdr->e_phoff - f->ehdr->e_phentsize;

    for (phdri = 0; phdri < f->ehdr->e_phnum; phdri++) {
        curphdrl += f->ehdr->e_phentsize;
        curphdr = (ElfNative_Phdr *) curphdrl;

        /* perhaps check its location */
        if (curphdr->p_type == PT_LOAD) {
            /* adjust min/max */
            if ((void *) curphdr->p_vaddr < f->min) {
                f->min = (void *) curphdr->p_vaddr;
            }
            if ((void *) curphdr->p_vaddr + curphdr->p_memsz > f->max) {
                f->max = (void *) curphdr->p_vaddr + curphdr->p_memsz;
            }
        }
    }

    /* with this size info, we can allocate the space */
    f->memsz = f->max - f->min;
    
    /* if this is a binary, try to allocate it in place. elfload is addressed above 0x18000000 */
    if (f->ehdr->e_type == ET_EXEC && f->max < (void *) 0x18000000) {
        f->loc = bbuffer(f->min, f->memsz);

    } else {
        f->loc = bbuffer(NULL, f->memsz);

    }
    memset(f->loc, 0, f->memsz);

    f->offset = f->loc - f->min;

    /* we have the space, so load it in */
    curphdrl = f->prog + f->ehdr->e_phoff - f->ehdr->e_phentsize;
    for (phdri = 0; phdri < f->ehdr->e_phnum; phdri++) {
        curphdrl += f->ehdr->e_phentsize;
        curphdr = (ElfNative_Phdr *) curphdrl;

        /* perhaps load it in */
        if (curphdr->p_type == PT_LOAD) {
            if (curphdr->p_filesz > 0) {
                /* OK, there's something to copy in, so do so */
                memcpy((void *) curphdr->p_vaddr + f->offset,
                       f->prog + curphdr->p_offset,
                       curphdr->p_filesz);
            }

        } else if (curphdr->p_type == PT_DYNAMIC) {
            /* we need this to load in dependencies, et cetera */
            f->dynamic = (ElfNative_Dyn *) (f->prog + curphdr->p_offset);

        }
    }
    
    /* now go through dynamic entries, looking for basic vital info */
    for (curdyn = f->dynamic; curdyn && curdyn->d_tag != DT_NULL; curdyn++) {
        if (curdyn->d_tag == DT_STRTAB) {
            f->strtab = (char *) (curdyn->d_un.d_ptr + f->offset);

        } else if (curdyn->d_tag == DT_SYMTAB) {
            f->symtab = (ElfNative_Sym *) (curdyn->d_un.d_ptr + f->offset);

        } else if (curdyn->d_tag == DT_HASH) {
            f->hashtab = (ElfNative_Word *) (curdyn->d_un.d_ptr + f->offset);

        } else if (curdyn->d_tag == DT_RELA) {
            f->rela = (ElfNative_Rela *) (curdyn->d_un.d_ptr + f->offset);

        } else if (curdyn->d_tag == DT_RELASZ) {
            f->relasz = curdyn->d_un.d_val;

        } else if (curdyn->d_tag == DT_REL) {
            f->rel = (ElfNative_Rel *) (curdyn->d_un.d_ptr + f->offset);

        } else if (curdyn->d_tag == DT_RELSZ) {
            f->relsz = curdyn->d_un.d_val;

        } else if (curdyn->d_tag == DT_JMPREL) {
            f->jmprel = (void *) (curdyn->d_un.d_ptr + f->offset);

        } else if (curdyn->d_tag == DT_PLTRELSZ) {
            f->jmprelsz = curdyn->d_un.d_val;

        }
    }

    /* load in dependencies */
    for (curdyn = f->dynamic; curdyn && curdyn->d_tag != DT_NULL; curdyn++) {
        if (curdyn->d_tag == DT_NEEDED) {
            loadELF(f->strtab + curdyn->d_un.d_val, instdir);
        }
    }

    return f;
}

void relocateELFs()
{
    int i;

    for (i = 0; i < elfFileCount; i++) {
        relocateELF(i, &(elfFiles[i]));
    }
}

void relocateELF(int fileNo, struct ELF_File *f)
{
    /* do processor-specific relocation */
#if (defined(__i386__) || defined(__i386) || defined(_M_IX86) || defined(_X86_)) && \
    !(defined(__x86_64__) || defined(_M_X64))
#define REL_P ((ssize_t) (currel->r_offset + f->offset))
#define REL_S ((ssize_t) (findELFSymbol( \
                f->strtab + f->symtab[ELFNATIVE_R_SYM(currel->r_info)].st_name, \
                NULL, fileNo, -1, NULL)))
#define REL_A (*((ssize_t *) REL_P))
#define WORD32_REL(to) REL_A = (ssize_t) (to)

    /* we ought to have rel and symtab defined */
    if (f->rel && f->symtab) {
        ElfNative_Rel *currel = f->rel;
        for (; (void *) currel < (void *) f->rel + f->relsz; currel++) {
            switch (ELFNATIVE_R_TYPE(currel->r_info)) {
                case R_386_32:
                    WORD32_REL(REL_S + REL_A);
                    break;

                case R_386_PC32:
                    WORD32_REL(REL_S + REL_A - REL_P);
                    break;

                case R_386_COPY:
                {
                    /* this is a bit more convoluted, as we need to find it in both places and copy */
                    ElfNative_Sym *localsym, *sosym;
                    localsym = &(f->symtab[ELFNATIVE_R_SYM(currel->r_info)]);
                    void *soptr = findELFSymbol(
                            f->strtab + localsym->st_name,
                            NULL, -1, fileNo, &sosym);

                    /* OK, we should have both, so copy it over */
                    if (localsym && sosym) {
                        memcpy((void *) (localsym->st_value + f->offset),
                               soptr, sosym->st_size);
                    } else {
                        /* depend on localsym's size */
                        memcpy((void *) (localsym->st_value + f->offset),
                               soptr, localsym->st_size);

                    }

                    break;
                }

                case R_386_GLOB_DAT:
                    WORD32_REL(f->loc + REL_A);
                    break;

                case R_386_RELATIVE:
                    WORD32_REL(f->loc + REL_A);
                    break;

                default:
                    fprintf(stderr, "Unsupported relocation %d in %s\n", ELFNATIVE_R_TYPE(currel->r_info), f->nm);
            }
        }
    }

    if (f->jmprel && f->symtab) {
        ElfNative_Rel *currel = (ElfNative_Rel *) f->jmprel;
        for (; (void *) currel < f->jmprel + f->jmprelsz; currel++) {
            switch (ELFNATIVE_R_TYPE(currel->r_info)) {
                case R_386_JMP_SLOT:
                    WORD32_REL(REL_S);
                    break;
            }
        }
    }


#elif defined(__x86_64__) || defined(_M_X64)
#define REL_P ((ssize_t) (currel->r_offset + f->offset))
#define REL_S ((ssize_t) (findELFSymbol( \
                f->strtab + f->symtab[ELFNATIVE_R_SYM(currel->r_info)].st_name, \
                NULL, fileNo, -1, NULL)))
#define REL_A (*((ssize_t *) REL_P))
#define WORD32_REL(to) REL_A = (int32_t) (to)
#define WORD64_REL(to) REL_A = (ssize_t) (to)

    /* we ought to have rel and symtab defined */
    if (f->rela && f->symtab) {
        ElfNative_Rela *currel = f->rela;
        for (; (void *) currel < (void *) f->rela + f->relasz; currel++) {
            switch (ELFNATIVE_R_TYPE(currel->r_info)) {
                case R_X86_64_64:
                    WORD64_REL(REL_S + REL_A);
                    break;

                case R_X86_64_PC32:
                    WORD32_REL(REL_S + REL_A - REL_P);
                    break;

                case R_X86_64_COPY:
                {
                    /* this is a bit more convoluted, as we need to find it in both places and copy */
                    ElfNative_Sym *localsym, *sosym;
                    localsym = &(f->symtab[ELFNATIVE_R_SYM(currel->r_info)]);
                    void *soptr = findELFSymbol(
                            f->strtab + localsym->st_name,
                            NULL, -1, fileNo, &sosym);

                    /* OK, we should have both, so copy it over */
                    if (localsym && sosym) {
                        memcpy((void *) (localsym->st_value + f->offset),
                               soptr, sosym->st_size);
                    } else {
                        /* depend on localsym's size */
                        memcpy((void *) (localsym->st_value + f->offset),
                               soptr, localsym->st_size);

                    }

                    break;
                }

                case R_X86_64_GLOB_DAT:
                    WORD64_REL(f->loc + REL_A);
                    break;

                case R_X86_64_RELATIVE:
                    WORD64_REL(f->loc + REL_A);
                    break;

                default:
                    fprintf(stderr, "Unsupported relocation %d in %s\n", (int) ELFNATIVE_R_TYPE(currel->r_info), f->nm);
            }
        }
    }

    if (f->jmprel && f->symtab) {
        ElfNative_Rela *currel = (ElfNative_Rela *) f->jmprel;
        for (; (void *) currel < (void *) f->jmprel + f->jmprelsz; currel++) {
            switch (ELFNATIVE_R_TYPE(currel->r_info)) {
                case R_X86_64_JUMP_SLOT:
                    WORD64_REL(REL_S);
                    break;

                default:
                    fprintf(stderr, "Unsupported jmprel relocation %d in %s\n", (int) ELFNATIVE_R_TYPE(currel->r_info), f->nm);
            }
        }
    }


#else
#error Unsupported architecture.
#endif
}

/* Initialize every ELF loaded /except/ for f (usually the binary) */
void initELF(struct ELF_File *except)
{
    int i;
    struct ELF_File *f;
    ElfNative_Dyn *dyn;

    for (i = elfFileCount - 1; i >= 0; i--) {
        f = &(elfFiles[i]);
        if (f == except) continue;

        /* init is in the dynamic section */
        if (f->dynamic == NULL) continue;
        for (dyn = f->dynamic; dyn && dyn->d_tag != DT_NULL; dyn++) {
            if (dyn->d_tag == DT_INIT) {
                /* call it */
                ((void(*)()) (dyn->d_un.d_ptr + f->offset))();
                break;
            }
        }
    }
}

/* Find a symbol within the currently loaded ELF files
 * localin: The number of the current file, where STB_LOCAL symbols are OK
 * notin: Do not bind to symbols in this file 
 * Either can be -1 */
void *findELFSymbol(const char *nm, struct ELF_File *onlyin, int localin, int notin, ElfNative_Sym **syminto)
{
    int i;
    struct ELF_File *f;
    ElfNative_Word hash = elf_hash((unsigned char *) nm);
    ElfNative_Word bucket, index;
    ElfNative_Sym *sym;
    void *hostsym;
    if (syminto) *syminto = NULL;

    if (nm[0] == '\0') return NULL;

    for (i = 0; i < elfFileCount; i++) {
        if (i == notin) continue;

        f = &(elfFiles[i]);
        if (onlyin && f != onlyin) continue;

        /* if this is a host library, just try the host method */
        if (f->hostlib == HOSTLIB_HOST) {
#if defined(HAVE_DLFCN_H)
            hostsym = dlsym(f->prog, nm);
            if (hostsym) return hostsym;
            continue;
#elif defined(__WIN32)
            char csym[1024];
            int isimp = 0;

            /* Remove _imp__ if it's present */
            if (strncmp(nm, "_imp__", 6) == 0) {
                isimp = 1;
                nm += 6;
            }

            /* Try adding a _ first, to get the cdecl version */
            snprintf(csym, 1024, "_%s", nm);
            hostsym = GetProcAddress(f->prog, csym);
            if (hostsym == NULL) {
                hostsym = GetProcAddress(f->prog, nm);
            }
            if (hostsym) {
                if (isimp) {
                    /* Need a pointer to this pointer */
                    void **pptr = (void **) malloc(sizeof(void*));
                    if (pptr == NULL) {
                        perror("malloc");
                        exit(1);
                    }
                    *pptr = hostsym;
                    return (void *) pptr;
                    
                } else {
                    return hostsym;

                }
            }
#endif
            continue;

        } else if (f->hostlib == HOSTLIB_DL) {
           hostsym = elfload_dl(nm);
           if (hostsym) return hostsym;
           continue;

        }

        /* figure out the bucket ... */
        bucket = hash % ELFFILE_NBUCKET(f);

        /* then find the chain entry */
        index = ELFFILE_BUCKET(f, bucket);

        /* and work our way through the chain */
        for (; index != STN_UNDEF; index = ELFFILE_CHAIN(f, index)) {
            sym = &(f->symtab[index]);

            /* see if it's defined */
            if (strcmp(f->strtab + sym->st_name, nm) == 0 &&
                (i == localin || ELFNATIVE_ST_BIND(sym->st_info) != STB_LOCAL) &&
                sym->st_shndx != SHN_UNDEF) {
                /* we found our symbol! */
                if (syminto != NULL) {
                    *syminto = sym;
                }
                return (void *) (sym->st_value + f->offset);
            }
        }
    }

    /* uh oh, not found! */
    fprintf(stderr, "Symbol undefined: '%s'\n", nm);
    return NULL;
}

/* The standard ELF hash function */
ElfNative_Word elf_hash(const unsigned char *name)
{
    ElfNative_Word h = 0, g;

    while (*name) {
        h = (h << 4) + *name++;
        if (g = h & 0xf0000000)
            h ^= g >> 24;
        h &= ~g;
    }
    return h;
}

/* A handy function to read a file or mmap it, as appropriate */
void readFile(const char *nm, const char *instdir, struct ELF_File *ef)
{
    /* try with instdir */
    char *longnm = malloc(strlen(nm) + strlen(instdir) + 18);
    if (longnm == NULL) {
        perror("malloc");
        exit(1);
    }
    sprintf(longnm, "%s/../lib/gelfload/%s", instdir, nm);

#ifdef HAVE_MMAP
{
    void *buf;
    struct stat sbuf;
    int fd;

    /* use mmap. First, open the file and get its length */
    fd = open(nm, O_RDONLY);
    if (fd == -1) {
        fd = open(longnm, O_RDONLY);

        if (fd == -1) {
            perror(nm);
            exit(1);
        }
    }
    free(longnm);
    if (fstat(fd, &sbuf) < 0) {
        perror(nm);
        exit(1);
    }

    /* then mmap it */
    buf = mmap(NULL, sbuf.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (buf == NULL) {
        perror("mmap");
        exit(1);
    }

    close(fd);

    /* and put it in ef */
    ef->prog = buf;
    ef->proglen = sbuf.st_size;
}
#else
{
    char *buf;
    int bufsz, rdtotal, rd;
    FILE *f;

    /* OK, use stdio */
    f = fopen(nm, "rb");
    if (f == NULL) {
        f = fopen(longnm, "rb");

        if (f == NULL) {
            perror(nm);
            exit(1);
        }
    }
    free(longnm);
    
    /* start with a 512-byte buffer */
    bufsz = 512;
    buf = (char *) malloc(bufsz);
    if (buf == NULL) {
        perror("malloc");
        exit(1);
    }

    /* and read in the file */
    rdtotal = 0;
    while ((rd = fread(buf + rdtotal, 1, bufsz - rdtotal, f)) != 0) {
        rdtotal += rd;
        if (rdtotal != bufsz) {
            /* done reading */
            break;

        } else {
            bufsz <<= 1;
            buf = realloc(buf, bufsz);
            if (buf == NULL) {
                perror("realloc");
                exit(1);
            }
        }
    }
    if (ferror(f)) {
        perror(nm);
        exit(1);
    }
    fclose(f);

    /* now put it in ef */
    ef->prog = buf;
    ef->proglen = rdtotal;
}
#endif
}

/* The finalization function for readFile */
void closeFile(struct ELF_File *ef)
{
#ifdef HAVE_MMAP
    munmap(ef->prog, ef->proglen);
#else
    free(ef->prog);
#endif
}
