#ifndef ELFNATIVE_H
#define ELFNATIVE_H

#include "../config.h"
#include "elfload_elf.h"

#if SIZEOF_VOID_P == 4

#define ElfNative_Ehdr Elf32_Ehdr
#define ElfNative_Phdr Elf32_Phdr
#define ElfNative_Dyn Elf32_Dyn
#define ElfNative_Sym Elf32_Sym
#define ElfNative_Word Elf32_Word
#define ElfNative_Rel Elf32_Rel
#define ElfNative_Rela Elf32_Rela

#define ELFNATIVE_ST_BIND ELF32_ST_BIND
#define ELFNATIVE_R_SYM ELF32_R_SYM
#define ELFNATIVE_R_TYPE ELF32_R_TYPE

#elif SIZEOF_VOID_P == 8

#define ElfNative_Ehdr Elf64_Ehdr
#define ElfNative_Phdr Elf64_Phdr
#define ElfNative_Dyn Elf64_Dyn
#define ElfNative_Sym Elf64_Sym
#define ElfNative_Word Elf64_Word
#define ElfNative_Rel Elf64_Rel
#define ElfNative_Rela Elf64_Rela

#define ELFNATIVE_ST_BIND ELF32_ST_BIND
#define ELFNATIVE_R_SYM ELF64_R_SYM
#define ELFNATIVE_R_TYPE ELF64_R_TYPE

#else

#error Unsupported bitwidth.

#endif

#endif
