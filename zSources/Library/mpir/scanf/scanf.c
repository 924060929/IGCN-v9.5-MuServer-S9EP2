//////////////////////////////////////////////////////////////////////
// scanf.c
/* gmp_scanf -- formatted input from stdin.

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

#include "config.h"

#if HAVE_STDARG
#include <stdarg.h>
#else
#include <varargs.h>
#endif

#include <stdio.h>

#include "mpir.h"
#include "gmp-impl.h"


int
#if HAVE_STDARG
gmp_scanf (const char *fmt, ...)
#else
gmp_scanf (va_alist)
     va_dcl
#endif
{
  va_list  ap;
  int      ret;
#if HAVE_STDARG
  va_start (ap, fmt);
#else
  const char *fmt;
  va_start (ap);
  fmt = va_arg (ap, const char *);
#endif

  ret = __gmp_doscan (&__gmp_fscanf_funs, stdin, fmt, ap);
  va_end (ap);
  return ret;
}


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

