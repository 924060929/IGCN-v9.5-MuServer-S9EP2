//////////////////////////////////////////////////////////////////////
// t-powm_ui.c
/* Test mpz_powm_ui, mpz_mul. mpz_mod.

Copyright 1991, 1993, 1994, 1996, 1997, 2000, 2001, 2002 Free Software
Foundation, Inc.

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

void dump_abort _PROTO ((mpz_t, mpz_t));
void debug_mp _PROTO ((mpz_t, int));

int
main (int argc, char **argv)
{
  mpz_t base, exp, mod;
  mpz_t r1, r2, base2;
  mp_size_t base_size, exp_size, mod_size;
  unsigned long int exp2;
  int i;
  int reps = 200;
  gmp_randstate_t rands;
  mpz_t bs;
  unsigned long bsi, size_range;

  tests_start ();
  gmp_randinit_default(rands);

  mpz_init (bs);

  if (argc == 2)
     reps = atoi (argv[1]);

  mpz_init (base);
  mpz_init (exp);
  mpz_init (mod);
  mpz_init (r1);
  mpz_init (r2);
  mpz_init (base2);

  for (i = 0; i < reps; i++)
    {
      mpz_urandomb (bs, rands, 32);
      size_range = mpz_get_ui (bs) % 13 + 2;

      do  /* Loop until mathematically well-defined.  */
	{
	  mpz_urandomb (bs, rands, size_range);
	  base_size = mpz_get_ui (bs);
	  mpz_rrandomb (base, rands, base_size);

	  mpz_urandomb (bs, rands, 6L);
	  exp_size = mpz_get_ui (bs);
	  mpz_rrandomb (exp, rands, exp_size);
	  exp2 = mpz_getlimbn (exp, (mp_size_t) 0);
	}
      while (mpz_cmp_ui (base, 0) == 0 && exp2 == 0);

      do
        {
	  mpz_urandomb (bs, rands, size_range);
	  mod_size = mpz_get_ui (bs);
	  mpz_rrandomb (mod, rands, mod_size);
	}
      while (mpz_cmp_ui (mod, 0) == 0);

      mpz_urandomb (bs, rands, 2);
      bsi = mpz_get_ui (bs);
      if ((bsi & 1) != 0)
	mpz_neg (base, base);

      /* printf ("%ld %ld\n", SIZ (base), SIZ (mod)); */

#if 0
      putc ('\n', stderr);
      debug_mp (base, -16);
      debug_mp (mod, -16);
#endif

      mpz_powm_ui (r1, base, exp2, mod);

      mpz_set_ui (r2, 1);
      mpz_set (base2, base);

      mpz_mod (r2, r2, mod);	/* needed when exp==0 and mod==1 */
      while (exp2 != 0)
	{
	  if (exp2 % 2 != 0)
	    {
	      mpz_mul (r2, r2, base2);
	      mpz_mod (r2, r2, mod);
	    }
	  mpz_mul (base2, base2, base2);
	  mpz_mod (base2, base2, mod);
	  exp2 = exp2 / 2;
	}

#if 0
      debug_mp (r1, -16);
      debug_mp (r2, -16);
#endif

      if (mpz_cmp (r1, r2) != 0)
	{
	  fprintf (stderr, "\ntest %d: Incorrect results for operands:\n", i);
	  debug_mp (base, -16);
	  debug_mp (exp, -16);
	  debug_mp (mod, -16);
	  fprintf (stderr, "mpz_powm_ui result:\n");
	  debug_mp (r1, -16);
	  fprintf (stderr, "reference result:\n");
	  debug_mp (r2, -16);
	  abort ();
	}
    }

  mpz_clear (bs);
  mpz_clear (base);
  mpz_clear (exp);
  mpz_clear (mod);
  mpz_clear (r1);
  mpz_clear (r2);
  mpz_clear (base2);
  gmp_randclear(rands);
  tests_end ();
  exit (0);
}

void
dump_abort (mpz_t dividend, mpz_t divisor)
{
  fprintf (stderr, "ERROR\n");
  fprintf (stderr, "dividend = "); debug_mp (dividend, -16);
  fprintf (stderr, "divisor  = "); debug_mp (divisor, -16);
  abort();
}

void
debug_mp (mpz_t x, int base)
{
  mpz_out_str (stderr, base, x); fputc ('\n', stderr);
}


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

