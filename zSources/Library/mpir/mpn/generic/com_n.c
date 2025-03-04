//////////////////////////////////////////////////////////////////////
// com_n.c
/* mpn_com - complement an mpn.

Copyright 2009 Free Software Foundation, Inc.

This file is part of the GNU MP Library.

The GNU MP Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 3 of the License, or (at your
option) any later version.

The GNU MP Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the GNU MP Library.  If not, see http://www.gnu.org/licenses/.  */

#include "mpir.h"
#include "gmp-impl.h"

#undef mpn_com_n
#define mpn_com_n __MPN(com_n)

void
mpn_com_n (mp_ptr rp, mp_srcptr up, mp_size_t n)
{
  mp_limb_t ul;
  do {
      ul = *up++;
      *rp++ = ~ul & GMP_NUMB_MASK;
  } while (--n != 0);
}


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

