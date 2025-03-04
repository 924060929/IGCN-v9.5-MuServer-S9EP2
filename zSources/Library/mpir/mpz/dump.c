//////////////////////////////////////////////////////////////////////
// dump.c
/* mpz_dump - Dump an integer to stdout.

   THIS IS AN INTERNAL FUNCTION WITH A MUTABLE INTERFACE.  IT IS NOT SAFE TO
   CALL THIS FUNCTION DIRECTLY.  IN FACT, IT IS ALMOST GUARANTEED THAT THIS
   FUNCTION WILL CHANGE OR DISAPPEAR IN A FUTURE GNU MP RELEASE.


Copyright 1999, 2000, 2001 Free Software Foundation, Inc.

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

#include <stdio.h>
#include <string.h> /* for strlen */
#include "mpir.h"
#include "gmp-impl.h"

void
mpz_dump (mpz_srcptr u)
{
  char *str;

  str = mpz_get_str (0, 10, u);
  printf ("%s\n", str);
  (*__gmp_free_func) (str, strlen (str) + 1);
}


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

