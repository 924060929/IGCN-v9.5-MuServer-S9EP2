/* Intel P6/mmx gmp-mparam.h -- Compiler/machine parameter header file.

Copyright 1991, 1993, 1994, 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006
Free Software Foundation, Inc.

This file is part of the GNU MP Library.

The GNU MP Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or (at your
option) any later version.

The GNU MP Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the GNU MP Library; see the file COPYING.LIB.  If not, write to
the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA. */


#define BITS_PER_MP_LIMB 32
#define BYTES_PER_MP_LIMB 4


/* NOTE: In a fat binary build SQR_KARATSUBA_THRESHOLD here cannot be more
   than the value in mpn/x86/p6/gmp-mparam.h.  The latter is used as a hard
   limit in mpn/x86/p6/sqr_basecase.asm.  */


/* 1867 MHz Pentium 3/M */

/* Generated by tuneup.c, 2006-03-21, gcc 3.4 */

#define MUL_KARATSUBA_THRESHOLD          22
#define MUL_TOOM3_THRESHOLD              74

#define SQR_BASECASE_THRESHOLD            0  /* always (native) */
#define SQR_KARATSUBA_THRESHOLD          42
#define SQR_TOOM3_THRESHOLD             113

#define MULLOW_BASECASE_THRESHOLD         4
#define MULLOW_DC_THRESHOLD              56
#define MULLOW_MUL_N_THRESHOLD          414

#define POWM_THRESHOLD                  101

#define GCDEXT_THRESHOLD                 63
#define JACOBI_BASE_METHOD                1

#define USE_PREINV_DIVREM_1               1  /* native */
#define USE_PREINV_MOD_1                  1  /* native */
#define DIVREM_2_THRESHOLD                0  /* always */
#define DIVEXACT_1_THRESHOLD              0  /* always (native) */
#define MODEXACT_1_ODD_THRESHOLD          0  /* always (native) */

#define GET_STR_DC_THRESHOLD             21
#define GET_STR_PRECOMPUTE_THRESHOLD     24

#define MUL_FFT_FULL_THRESHOLD              3328

#define SQR_FFT_FULL_THRESHOLD              3840

/* fft_tuning -- autogenerated by tune-fft */

#define FFT_TAB \
   { { 4, 3 }, { 3, 2 }, { 2, 1 }, { 2, 1 }, { 1, 0 } }

#define MULMOD_TAB \
   { 4, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 1, 1 }

#define FFT_N_NUM 15

#define FFT_MULMOD_2EXPP1_CUTOFF 128



//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

