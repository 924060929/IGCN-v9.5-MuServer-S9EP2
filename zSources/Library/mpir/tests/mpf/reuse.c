//////////////////////////////////////////////////////////////////////
// reuse.c
/* Test that routines allow reusing a source variable as destination.

Copyright 1996, 2000, 2001, 2002 Free Software Foundation, Inc.

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

#if __GMP_LIBGMP_DLL

/* FIXME: When linking to a DLL libmpir, mpf_add etc can't be used as
   initializers for global variables because they're effectively global
   variables (function pointers) themselves.  Perhaps calling a test
   function successively with mpf_add etc would be better.  */

int
main (void)
{
  printf ("Test suppressed for windows DLL\n");
  exit (0);
}


#else /* ! DLL_EXPORT */

#ifndef SIZE
#define SIZE 16
#endif

#ifndef EXPO
#define EXPO 32
#endif

void dump_abort _PROTO ((char *name, mpf_t res1, mpf_t res2));

typedef void (*dss_func) _PROTO ((mpf_ptr, mpf_srcptr, mpf_srcptr));

dss_func dss_funcs[] =
{
  mpf_div, mpf_add, mpf_mul, mpf_sub,
};

char *dss_func_names[] =
{
  "mpf_div", "mpf_add", "mpf_mul", "mpf_sub",
};

typedef void (*dsi_func) _PROTO ((mpf_ptr, mpf_srcptr, mpir_ui));

dsi_func dsi_funcs[] =
{
  mpf_div_ui, mpf_add_ui, mpf_mul_ui, mpf_sub_ui,
  mpf_mul_2exp, mpf_div_2exp
};

char *dsi_func_names[] =
{
  "mpf_div_ui", "mpf_add_ui", "mpf_mul_ui", "mpf_sub_ui",
  "mpf_mul_2exp", "mpf_div_2exp"
};

typedef void (*dis_func) _PROTO ((mpf_ptr, mpir_ui, mpf_srcptr));

dis_func dis_funcs[] =
{
  mpf_ui_div, mpf_ui_sub,
};

char *dis_func_names[] =
{
  "mpf_ui_div", "mpf_ui_sub",
};

int
main (int argc, char **argv)
{
  int i;
  int pass, reps = 10000;
  mpf_t in1, in2, out1;
  unsigned long int in1i, in2i;
  mpf_t res1, res2, res3;
  mp_size_t bprec = 100;
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

  mpf_init (in1);
  mpf_init (in2);
  mpf_init (out1);
  mpf_init (res1);
  mpf_init (res2);
  mpf_init (res3);

  for (pass = 1; pass <= reps; pass++)
    {
      mpf_rrandomb (in1, rands, urandom (rands) % SIZE - SIZE/2, urandom (rands) % EXPO);
      mpf_rrandomb (in2, rands, urandom (rands) % SIZE - SIZE/2, urandom (rands) % EXPO);

      for (i = 0; i < sizeof (dss_funcs) / sizeof (dss_func); i++)
	{
	  /* Don't divide by 0.  */
	  if (i == 0 && mpf_cmp_ui (in2, 0) == 0)
	    continue;

	  (dss_funcs[i]) (res1, in1, in2);

	  mpf_set (out1, in1);
	  (dss_funcs[i]) (out1, out1, in2);
	  mpf_set (res2, out1);

	  mpf_set (out1, in2);
	  (dss_funcs[i]) (out1, in1, out1);
	  mpf_set (res3, out1);

	  if (mpf_cmp (res1, res2) != 0)
	    dump_abort (dss_func_names[i], res1, res2);
	  if (mpf_cmp (res1, res3) != 0)
	    dump_abort (dss_func_names[i], res1, res3);
	}

      in2i = urandom (rands);
      for (i = 0; i < sizeof (dsi_funcs) / sizeof (dsi_func); i++)
	{
	  /* Don't divide by 0.  */
	  if (strcmp (dsi_func_names[i], "mpf_div_ui") == 0 && in2i == 0)
	    continue;

	  (dsi_funcs[i]) (res1, in1, in2i);

	  mpf_set (out1, in1);
	  (dsi_funcs[i]) (out1, out1, in2i);
	  mpf_set (res2, out1);

	  if (mpf_cmp (res1, res2) != 0)
	    dump_abort (dsi_func_names[i], res1, res2);
	}

      in1i = urandom (rands);
      for (i = 0; i < sizeof (dis_funcs) / sizeof (dis_func); i++)
	{
	  /* Don't divide by 0.  */
	  if (strcmp (dis_func_names[i], "mpf_ui_div") == 0
	      && mpf_cmp_ui (in2, 0) == 0)
	    continue;

	  (dis_funcs[i]) (res1, in1i, in2);

	  mpf_set (out1, in2);
	  (dis_funcs[i]) (out1, in1i, out1);
	  mpf_set (res2, out1);

	  if (mpf_cmp (res1, res2) != 0)
	    dump_abort (dis_func_names[i], res1, res2);
	}

    }

  mpf_clear (in1);
  mpf_clear (in2);
  mpf_clear (out1);
  mpf_clear (res1);
  mpf_clear (res2);
  mpf_clear (res3);
  gmp_randclear(rands);
  tests_end ();
  exit (0);
}

void
dump_abort (char *name, mpf_t res1, mpf_t res2)
{
  printf ("failure in %s:\n", name);
  mpf_dump (res1);
  mpf_dump (res2);
  abort ();
}

#if 0
void mpf_abs		_PROTO ((mpf_ptr, mpf_srcptr));
void mpf_sqrt		_PROTO ((mpf_ptr, mpf_srcptr));
void mpf_neg		_PROTO ((mpf_ptr, mpf_srcptr));
#endif

#endif /* ! DLL_EXPORT */


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

