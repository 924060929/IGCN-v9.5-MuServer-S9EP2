//////////////////////////////////////////////////////////////////////
// realloc.c
/* _mpz_realloc -- make the mpz_t have NEW_ALLOC digits allocated.

Copyright 1991, 1993, 1994, 1995, 2000, 2001 Free Software Foundation, Inc.

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

void *
_mpz_realloc (mpz_ptr m, mp_size_t new_alloc)
{
  mp_ptr mp;

  /* Never allocate zero space. */
  new_alloc = MAX (new_alloc, 1);

  mp = __GMP_REALLOCATE_FUNC_LIMBS (PTR(m), ALLOC(m), new_alloc);
  PTR(m) = mp;
  ALLOC(m) = new_alloc;

  /* Don't create an invalid number; if the current value doesn't fit after
     reallocation, clear it to 0.  */
  if (ABSIZ(m) > new_alloc)
    SIZ(m) = 0;

  return (void *) mp;
}


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

