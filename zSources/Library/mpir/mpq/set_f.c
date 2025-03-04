//////////////////////////////////////////////////////////////////////
// set_f.c
/* mpq_set_f -- set an mpq from an mpf.

Copyright 2000, 2001, 2002 Free Software Foundation, Inc.

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

#include "mpir.h"
#include "gmp-impl.h"
#include "longlong.h"


void
mpq_set_f (mpq_ptr q, mpf_srcptr f)
{
  mp_size_t  fexp = EXP(f);
  mp_ptr     fptr = PTR(f);
  mp_size_t  fsize = SIZ(f);
  mp_size_t  abs_fsize = ABS(fsize);
  mp_limb_t  flow;

  if (fsize == 0)
    {
      /* set q=0 */
      q->_mp_num._mp_size = 0;
      q->_mp_den._mp_size = 1;
      q->_mp_den._mp_d[0] = 1;
      return;
    }

  /* strip low zero limbs from f */
  flow = *fptr;
  MPN_STRIP_LOW_ZEROS_NOT_ZERO (fptr, abs_fsize, flow);

  if (fexp >= abs_fsize)
    {
      /* radix point is to the right of the limbs, no denominator */
      mp_ptr  num_ptr;

      MPZ_REALLOC (mpq_numref (q), fexp);
      num_ptr = q->_mp_num._mp_d;
      MPN_ZERO (num_ptr, fexp - abs_fsize);
      MPN_COPY (num_ptr + fexp - abs_fsize, fptr, abs_fsize);

      q->_mp_num._mp_size = fsize >= 0 ? fexp : -fexp;
      q->_mp_den._mp_size = 1;
      q->_mp_den._mp_d[0] = 1;
    }
  else
    {
      /* radix point is within or to the left of the limbs, use demominator */
      mp_ptr     num_ptr, den_ptr;
      mp_size_t  den_size;

      den_size = abs_fsize - fexp;
      MPZ_REALLOC (mpq_numref (q), abs_fsize);
      MPZ_REALLOC (mpq_denref (q), den_size+1);
      num_ptr = q->_mp_num._mp_d;
      den_ptr = q->_mp_den._mp_d;

      if (flow & 1)
        {
          /* no powers of two to strip from numerator */

          MPN_COPY (num_ptr, fptr, abs_fsize);
          MPN_ZERO (den_ptr, den_size);
          den_ptr[den_size] = 1;
        }
      else
        {
          /* right shift numerator, adjust denominator accordingly */
          int  shift;

          den_size--;
          count_trailing_zeros (shift, flow);

          mpn_rshift (num_ptr, fptr, abs_fsize, shift);
          abs_fsize -= (num_ptr[abs_fsize-1] == 0);

          MPN_ZERO (den_ptr, den_size);
          den_ptr[den_size] = GMP_LIMB_HIGHBIT >> (shift-1);
        }

      q->_mp_num._mp_size = fsize >= 0 ? abs_fsize : -abs_fsize;
      q->_mp_den._mp_size = den_size + 1;
    }
}


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

