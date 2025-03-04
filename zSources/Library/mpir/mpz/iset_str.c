//////////////////////////////////////////////////////////////////////
// iset_str.c
/* mpz_init_set_str(string, base) -- Convert the \0-terminated string
   STRING in base BASE to a multiple precision integer.  Return a mpz
   structure representing the integer.  Allow white space in the
   string.  If BASE == 0 determine the base in the C standard way,
   i.e.  0xhh...h means base 16, 0oo...o means base 8, otherwise
   assume base 10.

Copyright 1991, 1993, 1994, 1995, 2000, 2001, 2002 Free Software Foundation,
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

#include "mpir.h"
#include "gmp-impl.h"

int
mpz_init_set_str (mpz_ptr x, const char *str, int base)
{
  x->_mp_alloc = 1;
  x->_mp_d = (mp_ptr) (*__gmp_allocate_func) (BYTES_PER_MP_LIMB);

  /* if str has no digits mpz_set_str leaves x->_mp_size unset */
  x->_mp_size = 0;

#ifdef __CHECKER__
  /* let the low limb look initialized, for the benefit of mpz_get_ui etc */
  x->_mp_d[0] = 0;
#endif

  return mpz_set_str (x, str, base);
}


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

