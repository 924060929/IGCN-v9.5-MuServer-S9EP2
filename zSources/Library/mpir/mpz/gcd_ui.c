//////////////////////////////////////////////////////////////////////
// gcd_ui.c
/* mpz_gcd_ui -- Calculate the greatest common divisior of two integers.

Copyright 1994, 1996, 1999, 2000, 2001, 2002, 2003, 2004 Free Software
Foundation, Inc.

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

#include <stdio.h> /* for NULL */
#include "mpir.h"
#include "gmp-impl.h"

mpir_ui
mpz_gcd_ui (mpz_ptr w, mpz_srcptr u, mpir_ui v)
{
  mp_size_t un;
  mp_limb_t res;

#if BITS_PER_UI > GMP_NUMB_BITS  /* avoid warnings about shift amount */
  if (v > GMP_NUMB_MAX)
    {
      mpz_t vz;
      mp_limb_t vlimbs[2];
      vlimbs[0] = v & GMP_NUMB_MASK;
      vlimbs[1] = v >> GMP_NUMB_BITS;
      PTR(vz) = vlimbs;
      SIZ(vz) = 2;
      mpz_gcd (w, u, vz);
      /* because v!=0 we will have w<=v hence fitting a ulong */
      ASSERT (mpz_fits_ui_p (w));
      return mpz_get_ui (w);
    }
#endif

  un = ABSIZ(u);

  if (un == 0)
    res = v;
  else if (v == 0)
    {
      if (w != NULL)
	{
	  if (u != w)
	    {
	      MPZ_REALLOC (w, un);
	      MPN_COPY (PTR(w), PTR(u), un);
	    }
	  SIZ(w) = un;
	}
      /* Return u if it fits a ulong, otherwise 0. */
      res = PTR(u)[0];
      return (un == 1 && res <= ULONG_MAX ? res : 0);
    }
  else
    res = mpn_gcd_1 (PTR(u), un, (mp_limb_t) v);

  if (w != NULL)
    {
      PTR(w)[0] = res;
      SIZ(w) = res != 0;
    }
  return res;
}


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

