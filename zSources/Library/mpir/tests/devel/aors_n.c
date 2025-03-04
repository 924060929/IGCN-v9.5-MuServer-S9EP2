//////////////////////////////////////////////////////////////////////
// aors_n.c
/*
Copyright 1996, 1997, 1998, 1999, 2000, 2001, 2002, 2003, 2004 Free Software
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
MA 02110-1301, USA.
*/

#include <stdlib.h>
#include <stdio.h>
#include "mpir.h"
#include "gmp-impl.h"
#include "tests.h"

#ifdef OPERATION_add_n
#define func __gmpn_add_n
#define reffunc refmpn_add_n
#define funcname "mpn_add_n"
#endif

#ifdef OPERATION_sub_n
#define func __gmpn_sub_n
#define reffunc refmpn_sub_n
#define funcname "mpn_sub_n"
#endif

#ifdef OPERATION_addlsh1_n
#define func __gmpn_addlsh1_n
#define reffunc refmpn_addlsh1_n
#define funcname "mpn_addlsh1_n"
#endif

#ifdef OPERATION_sublsh1_n
#define func __gmpn_sublsh1_n
#define reffunc refmpn_sublsh1_n
#define funcname "mpn_sublsh1_n"
#endif

#ifdef OPERATION_rsh1add_n
#define func __gmpn_rsh1add_n
#define reffunc refmpn_rsh1add_n
#define funcname "mpn_rsh1add_n"
#endif

#ifdef OPERATION_rsh1sub_n
#define func __gmpn_rsh1sub_n
#define reffunc refmpn_rsh1sub_n
#define funcname "mpn_rsh1sub_n"
#endif

#if defined (USG) || defined (__SVR4) || defined (__hpux)
#include <time.h>

int
cputime ()
{
  if (CLOCKS_PER_SEC < 100000)
    return clock () * 1000 / CLOCKS_PER_SEC;
  return clock () / (CLOCKS_PER_SEC / 1000);
}
#else
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>

int
cputime ()
{
  struct rusage rus;

  getrusage (0, &rus);
  return rus.ru_utime.tv_sec * 1000 + rus.ru_utime.tv_usec / 1000;
}
#endif

static void mpn_print (mp_ptr, mp_size_t);

#define M * 1000000

#ifndef CLOCK
#error "Don't know CLOCK of your machine"
#endif

#ifndef OPS
#define OPS (CLOCK/5)
#endif
#ifndef SIZE
#define SIZE 328
#endif
#ifndef TIMES
#define TIMES OPS/(SIZE+1)
#endif

main (int argc, char **argv)
{
  mp_ptr s1, s2, dx, dy;
  mp_limb_t cyx, cyy;
  int i;
  long t0, t;
  unsigned int test;
  mp_size_t size;
  unsigned int ntests;
  gmp_randstate_t rands;
  gmp_randinit_default(rands);
  
  s1 = malloc (SIZE * sizeof (mp_limb_t));
  s2 = malloc (SIZE * sizeof (mp_limb_t));
  dx = malloc ((SIZE + 2) * sizeof (mp_limb_t));
  dy = malloc ((SIZE + 2) * sizeof (mp_limb_t));

  ntests = ~(unsigned) 0;
  if (argc == 2)
    ntests = strtol (argv[1], 0, 0);

  for (test = 1; test <= ntests; test++)
    {
#if TIMES == 1 && ! defined (PRINT)
      if (test % (SIZE > 100000 ? 1 : 100000 / SIZE) == 0)
	{
	  printf ("\r%u", test);
	  fflush (stdout);
	}
#endif

#ifdef RANDOM
      size = random () % SIZE + 1;
#else
      size = SIZE;
#endif

      dx[0] = 0x87654321;
      dy[0] = 0x87654321;
      dx[size+1] = 0x12345678;
      dy[size+1] = 0x12345678;

#if TIMES != 1
      mpn_randomb (s1, rands, size);
      mpn_randomb (s2, rands, size);

      t0 = cputime();
      for (i = 0; i < TIMES; i++)
	func (dx+1, s1, s2, size);
      t = cputime() - t0;
      printf (funcname ":    %5ldms (%.3f cycles/limb)\n",
	      t, ((double) t * CLOCK) / (TIMES * size * 1000.0));
#endif

#ifndef NOCHECK
      mpn_rrandom (s1, rands, size);
      mpn_rrandom (s2, rands, size);

#ifdef PRINT
      mpn_print (s1, size);
      mpn_print (s2, size);
#endif

      /* Put garbage in the destination.  */
      for (i = 0; i < size; i++)
	{
	  dx[i+1] = 0xdead;
	  dy[i+1] = 0xbeef;
	}

      cyx = reffunc (dx+1, s1, s2, size);
      cyy = func (dy+1, s1, s2, size);

#ifdef PRINT
      mpn_print (&cyx, 1);
      mpn_print (dx+1, size);
      mpn_print (&cyy, 1);
      mpn_print (dy+1, size);
#endif

      if (cyx != cyy || mpn_cmp (dx, dy, size+2) != 0
	  || dx[0] != 0x87654321 || dx[size+1] != 0x12345678)
	{
#ifndef PRINT
	  mpn_print (&cyx, 1);
	  mpn_print (dx+1, size);
	  mpn_print (&cyy, 1);
	  mpn_print (dy+1, size);
#endif
	  printf ("\n");
	  if (dy[0] != 0x87654321)
	    printf ("clobbered at low end\n");
	  if (dy[size+1] != 0x12345678)
	    printf ("clobbered at high end\n");
	  printf ("TEST NUMBER %u\n", test);
	  abort();
	}
#endif
    }
  exit (0);
}

static void
mpn_print (mp_ptr p, mp_size_t size)
{
  mp_size_t i;

  for (i = size - 1; i >= 0; i--)
    {
#ifdef _LONG_LONG_LIMB
      printf ("%0*lX%0*lX", (int) (sizeof(mp_limb_t)),
	      (unsigned long) (p[i] >> (BITS_PER_MP_LIMB/2)),
              (int) (sizeof(mp_limb_t)), (unsigned long) (p[i]));
#else
      printf ("%0*lX", (int) (2 * sizeof(mp_limb_t)), p[i]);
#endif
#ifdef SPACE
      if (i != 0)
	printf (" ");
#endif
    }
  puts ("");
}


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

