//////////////////////////////////////////////////////////////////////
// sub_ui.c
/* mpf_sub_ui -- Subtract an unsigned integer from a float.

Copyright 1993, 1994, 1996, 2001 Free Software Foundation, Inc.

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
mpf_sub_ui (mpf_ptr sum, mpf_srcptr u, mpir_ui v)
{
  __mpf_struct vv;
  mp_limb_t vl;

  if (v == 0)
    {
      mpf_set (sum, u);
      return;
    }

  vl = v;
  vv._mp_size = 1;
  vv._mp_d = &vl;
  vv._mp_exp = 1;
  mpf_sub (sum, u, &vv);
}


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

