/***************************************************************
 *         aPLib v0.20b  -  the smaller the better :)          *
 *                                                             *
 *   Copyright (c) 1998-99 by  ÄúJibzúÄ  All Rights Reserved   *
 ***************************************************************/

#ifndef __APLIB_H
#define __APLIB_H

#ifdef __cplusplus
extern "C" {
#endif

unsigned int __cdecl aP_pack(unsigned char *source,
                             unsigned char *destination,
                             unsigned int length,
                             unsigned char *workmem,
                             int (__cdecl *callback) (unsigned int, unsigned int));

unsigned int __cdecl aP_depack_asm(unsigned char *, unsigned char *);

unsigned int __cdecl aP_depack_asm_fast(unsigned char *, unsigned char *);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
