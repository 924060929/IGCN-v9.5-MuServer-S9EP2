//////////////////////////////////////////////////////////////////////
// abs.c
/* mpf_abs -- Compute the absolute value of a float.

Copyright 1993, 1994, 1995, 2001 Free Software Foundation, Inc.

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

void
mpf_abs (mpf_ptr r, mpf_srcptr u)
{
  mp_size_t size;

  size = ABS (u->_mp_size);
  if (r != u)
    {
      mp_size_t prec;
      mp_ptr rp, up;

      prec = r->_mp_prec + 1;	/* lie not to lose precision in assignment */
      rp = r->_mp_d;
      up = u->_mp_d;

      if (size > prec)
	{
	  up += size - prec;
	  size = prec;
	}

      MPN_COPY (rp, up, size);
      r->_mp_exp = u->_mp_exp;
    }
  r->_mp_size = size;
}


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

