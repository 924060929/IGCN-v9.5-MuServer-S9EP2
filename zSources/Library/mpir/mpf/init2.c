//////////////////////////////////////////////////////////////////////
// init2.c
/* mpf_init2() -- Make a new multiple precision number with value 0.

Copyright 1993, 1994, 1995, 2000, 2001, 2004 Free Software Foundation, Inc.

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
mpf_init2 (mpf_ptr r, mp_bitcnt_t prec_in_bits)
{
  mp_size_t prec;

  prec = __GMPF_BITS_TO_PREC (prec_in_bits);
  r->_mp_size = 0;
  r->_mp_exp = 0;
  r->_mp_prec = prec;
  r->_mp_d = (mp_ptr) (*__gmp_allocate_func) ((prec + 1) * BYTES_PER_MP_LIMB);
}


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

