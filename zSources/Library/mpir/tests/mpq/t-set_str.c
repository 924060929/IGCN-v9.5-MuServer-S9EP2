//////////////////////////////////////////////////////////////////////
// t-set_str.c
/* Test mpq_set_str.

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
check_one (mpq_srcptr want, int base, const char *str)
{
  mpq_t   got;
  
  MPQ_CHECK_FORMAT (want);
  mp_trace_base = base;

  mpq_init (got);

  if (mpq_set_str (got, str, base) != 0)
    {
      printf ("mpq_set_str unexpectedly failed\n");
      printf ("  base %d\n", base);
      printf ("  str  \"%s\"\n", str);
      abort ();
    }
  MPQ_CHECK_FORMAT (got);

  if (! mpq_equal (got, want))
    {
      printf ("mpq_set_str wrong\n");
      printf ("  base %d\n", base);
      printf ("  str  \"%s\"\n", str);
      mpq_trace ("got ", got);
      mpq_trace ("want", want);
      abort ();
    }

  mpq_clear (got);
}

void
check_samples (void)
{
  mpq_t  q;

  mpq_init (q);

  mpq_set_ui (q, 0L, 1L);
  check_one (q, 10, "0");
  check_one (q, 10, "0/1");
  check_one (q, 10, "0  / 1");
  check_one (q, 0, "0x0/ 1");
  check_one (q, 0, "0x0/ 0x1");
  check_one (q, 0, "0 / 0x1");

  check_one (q, 10, "-0");
  check_one (q, 10, "-0/1");
  check_one (q, 10, "-0  / 1");
  check_one (q, 0, "-0x0/ 1");
  check_one (q, 0, "-0x0/ 0x1");
  check_one (q, 0, "-0 / 0x1");

  mpq_set_ui (q, 255L, 256L);
  check_one (q, 10, "255/256");
  check_one (q, 0,  "0xFF/0x100");
  check_one (q, 16, "FF/100");

  mpq_neg (q, q);
  check_one (q, 10, "-255/256");
  check_one (q, 0,  "-0xFF/0x100");
  check_one (q, 16, "-FF/100");

  mpq_clear (q);
}

int
main (void)
{
  tests_start ();

  check_samples ();

  tests_end ();
  exit (0);
}


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

