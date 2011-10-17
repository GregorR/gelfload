#ifndef SHIM_H
#define SHIM_H

/* shim this symbol */
#define SHIM(nm) __gelfload_shim_ ## nm

/* make this function a constructor (GCC-only for now) */
#define CONSTRUCTOR(f) __attribute__((constructor)) f

/* make an unimplemented shim */
#define UNIMPL_SHIM(nm) int SHIM(nm)() { fprintf(stderr, "Shim unimplemented: %s\n", #nm); return 0; }

/* make a direct shim (for functions which may be macros in some places) */
#define DIRECT_INT_FUNC(nm) int SHIM(nm)(void (*a)()) { return nm(a); }

/* shim this type (give a target type) */
#define TSHIM(nm) struct __gelfload_shim_ ## nm

/* 'new' for shim */
#define TSHIM_NEW(nm) __gelfload_tshim_new_ ## nm
#define TSHIM_NEW_D(nm) TSHIM(nm) *TSHIM_NEW(nm)()

/* 'delete' for shim */
#define TSHIM_DELETE(nm) __gelfload_tshim_delete_ ## nm
#define TSHIM_DELETE_D(nm) void TSHIM_DELETE(nm)(TSHIM(nm) *t)

/* target->host conversion */
#define TSHIM_T2H(nm) __gelfload_tshim_t2h_ ## nm
#define TSHIM_T2H_D(nm) void TSHIM_T2H(nm)(TSHIM(nm) **t, nm **h)

/* host->target conversion */
#define TSHIM_H2T(nm) __gelfload_tshim_h2t_ ## nm
#define TSHIM_H2T_D(nm) void TSHIM_H2T(nm)(nm **h, TSHIM(nm) **t)

#endif
