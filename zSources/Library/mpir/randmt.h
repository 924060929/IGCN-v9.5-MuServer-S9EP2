//////////////////////////////////////////////////////////////////////
// randmt.h
/* Mersenne Twister pseudo-random number generator defines.

Copyright 2002, 2003 Free Software Foundation, Inc.

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
MA 02110-1301, USA.  */


/* Number of extractions used to warm the buffer up.  */
#define WARM_UP 2000

/* Period parameters.  */
#define N 624
#define M 397
#define MATRIX_A 0x9908B0DF   /* Constant vector a.  */

/* State structure for MT.  */
typedef struct
{
  gmp_uint_least32_t mt[N];    /* State array.  */
  int mti;                     /* Index of current value.  */
} gmp_rand_mt_struct;


void __gmp_mt_recalc_buffer __GMP_PROTO ((gmp_uint_least32_t *));
void __gmp_randget_mt __GMP_PROTO ((gmp_randstate_t, mp_ptr, mpir_ui));
void __gmp_randclear_mt __GMP_PROTO ((gmp_randstate_t rstate));
void __gmp_randiset_mt __GMP_PROTO ((gmp_randstate_ptr, gmp_randstate_srcptr));


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

