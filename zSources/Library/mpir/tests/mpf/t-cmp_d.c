//////////////////////////////////////////////////////////////////////
// t-cmp_d.c
/* Test mpf_cmp_d.

Copyright 2001, 2003, 2003, 2005 Free Software Foundation, Inc.

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
#include <string.h>

#include "mpir.h"
#include "gmp-impl.h"
#include "tests.h"


#define SGN(n)  ((n) > 0 ? 1 : (n) < 0 ? -1 : 0)

void
check_one (const char *name, mpf_srcptr x, double y, int cmp)
{
  int   got;

  got = mpf_cmp_d (x, y);
  if (SGN(got) != cmp)
    {
      int i;
      printf    ("mpf_cmp_d wrong (from %s)\n", name);
      printf    ("  got  %d\n", got);
      printf    ("  want %d\n", cmp);
      mpf_trace ("  x", x);
      printf    ("  y %g\n", y);
      mp_trace_base=-16;
      mpf_trace ("  x", x);
      printf    ("  y %g\n", y);
      printf    ("  y");
      for (i = 0; i < sizeof(y); i++)
        printf (" %02X", (unsigned) ((unsigned char *) &y)[i]);
      printf ("\n");
      abort ();
    }
}

void
check_infinity (void)
{
  mpf_t   x;
  double  y = tests_infinity_d ();
  if (y == 0.0)
    return;

  mpf_init (x);

  /* 0 cmp inf */
  mpf_set_ui (x, 0L);
  check_one ("check_infinity", x,  y, -1);
  check_one ("check_infinity", x, -y,  1);

  /* 123 cmp inf */
  mpf_set_ui (x, 123L);
  check_one ("check_infinity", x,  y, -1);
  check_one ("check_infinity", x, -y,  1);

  /* -123 cmp inf */
  mpf_set_si (x, -123L);
  check_one ("check_infinity", x,  y, -1);
  check_one ("check_infinity", x, -y,  1);

  /* 2^5000 cmp inf */
  mpf_set_ui (x, 1L);
  mpf_mul_2exp (x, x, 5000L);
  check_one ("check_infinity", x,  y, -1);
  check_one ("check_infinity", x, -y,  1);

  /* -2^5000 cmp inf */
  mpf_neg (x, x);
  check_one ("check_infinity", x,  y, -1);
  check_one ("check_infinity", x, -y,  1);

  mpf_clear (x);
}

int
main (int argc, char *argv[])
{
  tests_start ();

  check_infinity ();

  tests_end ();
  exit (0);
}


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

