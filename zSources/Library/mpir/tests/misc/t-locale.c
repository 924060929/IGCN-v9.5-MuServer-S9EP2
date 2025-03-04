//////////////////////////////////////////////////////////////////////
// t-locale.c
/* Test locale support, or attempt to do so.

Copyright 2001, 2002 Free Software Foundation, Inc.

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

#ifndef _GNU_SOURCE
#define _GNU_SOURCE    /* for DECIMAL_POINT in glibc langinfo.h */
#endif

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if HAVE_NL_TYPES_H
#include <nl_types.h>  /* for nl_item (on netbsd 1.4.1 at least) */
#endif

#if HAVE_LANGINFO_H
#include <langinfo.h>  /* for nl_langinfo */
#endif

#if HAVE_LOCALE_H
#include <locale.h>    /* for lconv */
#endif

#include "mpir.h"
#include "gmp-impl.h"
#include "tests.h"


char *decimal_point;

/* Replace the libc localeconv with one we can manipulate. */
/*
The t-local test fails on
mingw64   ie defined(_WIN64) && !defined(_MSC_VER)

  to detect a non-msvc 64bit windows the above is the best solution ,
  but as we are using it to exclude a test , 
  it would be better to only limit it to mingw64 only and not some 
  other future 64bit windows gcc , so it's best in this case to use the 
  defined(__MINGW64__) macro

msvc with version<=1500  ie defined(_MSC_VER) && _MSC_VER <= 1500

*/

#if _MSC_VER && __GMP_LIBGMP_DLL

int
main (void)
{
  printf ("Test suppressed for windows DLL\n");
  exit (0);
}


#else /* ! DLL_EXPORT */

#if ! (defined(__MINGW64_VERSION_MAJOR) || (defined(_MSC_VER) && _MSC_VER <= 1500))
#if HAVE_LOCALECONV
#ifdef _MSC_VER
__GMP_DECLSPEC
#endif
struct lconv *
localeconv (void)
{
  static struct lconv  l;
  l.decimal_point = decimal_point;
  return &l;
}
#endif
#endif

/* Replace the libc nl_langinfo with one we can manipulate. */
#if HAVE_NL_LANGINFO
char *
nl_langinfo (nl_item n)
{
#if defined (DECIMAL_POINT)
  if (n == DECIMAL_POINT)
    return decimal_point;
#endif
#if defined (RADIXCHAR)
  if (n == RADIXCHAR)
    return decimal_point;
#endif
  return "";
}
#endif

void
check_input (void)
{
  static char *point[] = {
    ".", ",", "WU", "STR", "ZTV***"
  };

  static const struct {
    const char  *str;
    double      d;
  } data[] = {

    { "1%s",   1.0 },
    { "1%s0",  1.0 },
    { "1%s00", 1.0 },

    { "%s5",    0.5 },
    { "0%s5",   0.5 },
    { "00%s5",  0.5 },
    { "00%s50", 0.5 },

    { "1%s5",    1.5 },
    { "1%s5e1", 15.0 },
  };

  int     i, j, neg, ret;
  char    str[128];
  mpf_t   f;
  double  d;

  mpf_init (f);

  for (i = 0; i < numberof (point); i++)
    {
      decimal_point = point[i];

      for (neg = 0; neg <= 1; neg++)
        {
          for (j = 0; j < numberof (data); j++)
            {
              strcpy (str, neg ? "-" : "");
              sprintf (str+strlen(str), data[j].str, decimal_point);

              d = data[j].d;
              if (neg)
                d = -d;

              mpf_set_d (f, 123.0);
              if (mpf_set_str (f, str, 10) != 0)
                {
                  printf ("mpf_set_str error\n");
                  printf ("  point  %s\n", decimal_point);
                  printf ("  str    %s\n", str);
                  abort ();
                }
              if (mpf_cmp_d (f, d) != 0)
                {
                  printf    ("mpf_set_str wrong result\n");
                  printf    ("  point  %s\n", decimal_point);
                  printf    ("  str    %s\n", str);
                  mpf_trace ("  f", f);
                  printf    ("  d=%g\n", d);
                  abort ();
                }

              mpf_set_d (f, 123.0);
              ret = gmp_sscanf (str, "%Ff", f);
              if (ret != 1)
                {
                  printf ("gmp_sscanf wrong return value\n");
                  printf ("  point  %s\n", decimal_point);
                  printf ("  str    %s\n", str);
                  printf ("  ret    %d\n", ret);
                  abort ();
                }
              if (mpf_cmp_d (f, d) != 0)
                {
                  printf    ("gmp_sscanf wrong result\n");
                  printf    ("  point  %s\n", decimal_point);
                  printf    ("  str    %s\n", str);
                  mpf_trace ("  f", f);
                  printf    ("  d=%g\n", d);
                  abort ();
                }
            }
        }
    }
  mpf_clear (f);
}

int
main (void)
{
  /* The localeconv replacement breaks printf "%lu" on SunOS 4, so we can't
     print the seed in tests_rand_start().  Nothing random is used in this
     program though, so just use the memory tests alone.  */
  tests_memory_start ();

  {
    mpf_t  f;
    char   buf[128];
    mpf_init (f);
    decimal_point = ",";
    mpf_set_d (f, 1.5);
    gmp_snprintf (buf, sizeof(buf), "%.1Ff", f);
    mpf_clear (f);
    if (strcmp (buf, "1,5") != 0)
      {
        printf ("Test skipped, replacing localeconv/nl_langinfo doesn't work\n");
        goto done;
      }
  }

  check_input ();

 done:
  tests_memory_end ();
  exit (0);
}

#endif


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

