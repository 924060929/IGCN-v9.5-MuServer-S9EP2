//////////////////////////////////////////////////////////////////////
// t-add.c
/* Test mpf_add.

Copyright 1996, 2001 Free Software Foundation, Inc.

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

#ifndef SIZE
#define SIZE 16
#endif

int
main (int argc, char **argv)
{
  mp_size_t size;
  mp_exp_t exp;
  int reps = 20000;
  int i;
  mpf_t u, v, w, wref;
  mp_size_t bprec = 100;
  mpf_t rerr, max_rerr, limit_rerr;
  gmp_randstate_t rands;
  
  tests_start ();
  gmp_randinit_default(rands);
  if (argc > 1)
    {
      reps = strtol (argv[1], 0, 0);
      if (argc > 2)
	bprec = strtol (argv[2], 0, 0);
    }

  mpf_set_default_prec (bprec);

  mpf_init_set_ui (limit_rerr, 1);
  mpf_div_2exp (limit_rerr, limit_rerr, bprec);
#if VERBOSE
  mpf_dump (limit_rerr);
#endif
  mpf_init (rerr);
  mpf_init_set_ui (max_rerr, 0);

  mpf_init (u);
  mpf_init (v);
  mpf_init (w);
  mpf_init (wref);
  for (i = 0; i < reps; i++)
    {
      size = urandom (rands) % (2 * SIZE) - SIZE;
      exp = urandom (rands) % SIZE;
      mpf_rrandomb (u, rands, size, exp);

      size = urandom (rands) % (2 * SIZE) - SIZE;
      exp = urandom (rands) % SIZE;
      mpf_rrandomb (v, rands, size, exp);

      mpf_add (w, u, v);
      refmpf_add (wref, u, v);

      mpf_reldiff (rerr, w, wref);
      if (mpf_cmp (rerr, max_rerr) > 0)
	{
	  mpf_set (max_rerr, rerr);
#if VERBOSE
	  mpf_dump (max_rerr);
#endif
	  if (mpf_cmp (rerr, limit_rerr) > 0)
	    {
	      printf ("ERROR after %d tests\n", i);
	      printf ("   u = "); mpf_dump (u);
	      printf ("   v = "); mpf_dump (v);
	      printf ("wref = "); mpf_dump (wref);
	      printf ("   w = "); mpf_dump (w);
	      abort ();
	    }
	}
    }

  mpf_clear (limit_rerr);
  mpf_clear (rerr);
  mpf_clear (max_rerr);

  mpf_clear (u);
  mpf_clear (v);
  mpf_clear (w);
  mpf_clear (wref);
  gmp_randclear(rands);
  tests_end ();
  exit (0);
}


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

