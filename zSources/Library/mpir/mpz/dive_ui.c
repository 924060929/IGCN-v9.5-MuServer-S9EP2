//////////////////////////////////////////////////////////////////////
// dive_ui.c
/* mpz_divexact_ui -- exact division mpz by ulong.

Copyright 2001, 2002 Free Software Foundation, Inc.

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
MA 02110-1301, USA.
*/

#include "mpir.h"
#include "gmp-impl.h"

void
mpz_divexact_ui (mpz_ptr dst, mpz_srcptr src, mpir_ui divisor)
{
  mp_size_t  size, abs_size;
  mp_ptr     dst_ptr;

  if (divisor == 0)
    DIVIDE_BY_ZERO;

  /* For nails don't try to be clever if d is bigger than a limb, just fake
     up an mpz_t and go to the main mpz_divexact.  */
  if (divisor > GMP_NUMB_MAX)
    {
      mp_limb_t  dlimbs[2];
      mpz_t      dz;
      ALLOC(dz) = 2;
      PTR(dz) = dlimbs;
      mpz_set_ui (dz, divisor);
      mpz_divexact (dst, src, dz);
      return;
    }

  size = SIZ(src);
  if (size == 0)
    {
      SIZ(dst) = 0;
      return;
    }
  abs_size = ABS (size);

  MPZ_REALLOC (dst, abs_size);
  dst_ptr = PTR(dst);

  MPN_DIVREM_OR_DIVEXACT_1 (dst_ptr, PTR(src), abs_size, (mp_limb_t) divisor);
  abs_size -= (dst_ptr[abs_size-1] == 0);
  SIZ(dst) = (size >= 0 ? abs_size : -abs_size);
}


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

