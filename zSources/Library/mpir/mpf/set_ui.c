//////////////////////////////////////////////////////////////////////
// set_ui.c
/* mpf_set_ui() -- Assign a float from an unsigned int.

Copyright 1993, 1994, 1995, 2001, 2002, 2004 Free Software Foundation, Inc.

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
mpf_set_ui (mpf_ptr f, mpir_ui val)
{
  mp_size_t size;

  f->_mp_d[0] = val & GMP_NUMB_MASK;
  size = val != 0;

#if BITS_PER_UI > GMP_NUMB_BITS
  val >>= GMP_NUMB_BITS;
  f->_mp_d[1] = val;
  size += (val != 0);
#endif

  f->_mp_exp = f->_mp_size = size;
}


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

