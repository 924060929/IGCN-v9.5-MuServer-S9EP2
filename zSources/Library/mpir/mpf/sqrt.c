//////////////////////////////////////////////////////////////////////
// sqrt.c
/* mpf_sqrt -- Compute the square root of a float.

Copyright 1993, 1994, 1996, 2000, 2001, 2004, 2005 Free Software Foundation,
Inc.

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


/* As usual, the aim is to produce PREC(r) limbs of result, with the high
   limb non-zero.  This is accomplished by applying mpn_sqrtrem to either
   2*prec or 2*prec-1 limbs, both such sizes resulting in prec limbs.

   The choice between 2*prec or 2*prec-1 limbs is based on the input
   exponent.  With b=2^GMP_NUMB_BITS the limb base then we can think of
   effectively taking out a factor b^(2k), for suitable k, to get to an
   integer input of the desired size ready for mpn_sqrtrem.  It must be an
   even power taken out, ie. an even number of limbs, so the square root
   gives factor b^k and the radix point is still on a limb boundary.  So if
   EXP(r) is even we'll get an even number of input limbs 2*prec, or if
   EXP(r) is odd we get an odd number 2*prec-1.

   Further limbs below the 2*prec or 2*prec-1 used don't affect the result
   and are simply truncated.  This can be seen by considering an integer x,
   with s=floor(sqrt(x)).  s is the unique integer satisfying s^2 <= x <
   (s+1)^2.  Notice that adding a fraction part to x (ie. some further bits)
   doesn't change the inequality, s remains the unique solution.  Working
   suitable factors of 2 into this argument lets it apply to an intended
   precision at any position for any x, not just the integer binary point.

   If the input is smaller than 2*prec or 2*prec-1, then we just pad with
   zeros, that of course being our usual interpretation of short inputs.
   The effect is to extend the root beyond the size of the input (for
   instance into fractional limbs if u is an integer).  */

void
mpf_sqrt (mpf_ptr r, mpf_srcptr u)
{
  mp_size_t usize;
  mp_ptr up, tp;
  mp_size_t prec, tsize;
  mp_exp_t uexp, expodd;
  TMP_DECL;

  usize = u->_mp_size;
  if (usize <= 0)
    {
      if (usize < 0)
        SQRT_OF_NEGATIVE;
      r->_mp_size = 0;
      r->_mp_exp = 0;
      return;
    }

  TMP_MARK;

  uexp = u->_mp_exp;
  prec = r->_mp_prec;
  up = u->_mp_d;

  expodd = (uexp & 1);
  tsize = 2 * prec - expodd;
  r->_mp_size = prec;
  r->_mp_exp = (uexp + expodd) / 2;    /* ceil(uexp/2) */

  /* root size is ceil(tsize/2), this will be our desired "prec" limbs */
  ASSERT ((tsize + 1) / 2 == prec);

  tp = (mp_ptr) TMP_ALLOC (tsize * BYTES_PER_MP_LIMB);

  if (usize > tsize)
    {
      up += usize - tsize;
      usize = tsize;
      MPN_COPY (tp, up, tsize);
    }
  else
    {
      MPN_ZERO (tp, tsize - usize);
      MPN_COPY (tp + (tsize - usize), up, usize);
    }

  mpn_sqrtrem (r->_mp_d, NULL, tp, tsize);

  TMP_FREE;
}


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

