//////////////////////////////////////////////////////////////////////
// set_z.c
/* mpf_set_z -- Assign a float from an integer.

Copyright 1996, 2001, 2004 Free Software Foundation, Inc.

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
mpf_set_z (mpf_ptr r, mpz_srcptr u)
{
  mp_ptr rp, up;
  mp_size_t size, asize;
  mp_size_t prec;

  prec = PREC (r) + 1;
  size = SIZ (u);
  asize = ABS (size);
  rp = PTR (r);
  up = PTR (u);

  EXP (r) = asize;

  if (asize > prec)
    {
      up += asize - prec;
      asize = prec;
    }

  SIZ (r) = size >= 0 ? asize : -asize;
  MPN_COPY (rp, up, asize);
}


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

