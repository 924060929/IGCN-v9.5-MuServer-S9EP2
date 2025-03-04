//////////////////////////////////////////////////////////////////////
// t-int_p.c
/* Test mpf_integer_p.

Copyright 2001 Free Software Foundation, Inc.

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
#include <stdlib.h>
#include "mpir.h"
#include "gmp-impl.h"
#include "tests.h"


void
one (mpf_srcptr f, int want)
{
  int  got;
  got = mpf_integer_p (f);
  if (got != want)
    {
      printf ("mpf_integer_p got %d want %d\n", got, want);
      mpf_trace (" f", f);
      abort ();
    }
}

void
all (mpf_ptr f, int want)
{
  one (f, want);
  mpf_neg (f, f);
  one (f, want);
}

int
main (void)
{
  mpf_t  f;

  tests_start ();
  mpf_init2 (f, 200L);

  mpf_set_ui (f, 0L);
  one (f, 1);

  mpf_set_ui (f, 1L);
  all (f, 1);

  mpf_set_ui (f, 1L);
  mpf_div_2exp (f, f, 1L);
  all (f, 0);

  mpf_set_ui (f, 1L);
  mpf_div_2exp (f, f, 5000L);
  all (f, 0);

  mpf_set_ui (f, 1L);
  mpf_mul_2exp (f, f, 5000L);
  all (f, 1);

  mpf_set_str (f, "0.5", 10);
  all (f, 0);

  mpf_set_ui (f, 1L);
  mpf_div_ui (f, f, 3L);
  all (f, 0);

  mpf_clear (f);
  tests_end ();
  exit (0);
}


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

