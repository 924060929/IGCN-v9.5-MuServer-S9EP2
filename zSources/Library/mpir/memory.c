//////////////////////////////////////////////////////////////////////
// memory.c
/* Memory allocation routines.

Copyright 1991, 1993, 1994, 2000, 2001, 2002 Free Software Foundation, Inc.

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
#include <stdlib.h> /* for malloc, realloc, free */

#include "mpir.h"
#include "gmp-impl.h"


void *	(*__gmp_allocate_func) _PROTO ((size_t)) = __gmp_default_allocate;
void *	(*__gmp_reallocate_func) _PROTO ((void *, size_t, size_t))
     = __gmp_default_reallocate;
void	(*__gmp_free_func) _PROTO ((void *, size_t)) = __gmp_default_free;


/* Default allocation functions.  In case of failure to allocate/reallocate
   an error message is written to stderr and the program aborts.  */

void *
__gmp_default_allocate (size_t size)
{
  void *ret;
#ifdef DEBUG
  size_t req_size = size;
  size += 2 * BYTES_PER_MP_LIMB;
#endif
  ret = malloc (size);
  if (ret == 0)
    {
#if defined _MSC_VER && defined _WIN64
      fprintf (stderr, "GNU MP: Cannot allocate memory (size=%llu)\n", size);
#else
      fprintf (stderr, "GNU MP: Cannot allocate memory (size=%lu)\n", size);
#endif
      abort ();
    }

#ifdef DEBUG
  {
    mp_ptr p = ret;
    p++;
    p[-1] = (0xdeadbeef << 31) + 0xdeafdeed;
    if (req_size % BYTES_PER_MP_LIMB == 0)
      p[req_size / BYTES_PER_MP_LIMB] = ~((0xdeadbeef << 31) + 0xdeafdeed);
    ret = p;
  }
#endif
  return ret;
}

void *
__gmp_default_reallocate (void *oldptr, size_t old_size, size_t new_size)
{
  void *ret;

#ifdef DEBUG
  size_t req_size = new_size;

  if (old_size != 0)
    {
      mp_ptr p = oldptr;
      if (p[-1] != (0xdeadbeef << 31) + 0xdeafdeed)
	{
	  fprintf (stderr, "gmp: (realloc) data clobbered before allocation block\n");
	  abort ();
	}
      if (old_size % BYTES_PER_MP_LIMB == 0)
	if (p[old_size / BYTES_PER_MP_LIMB] != ~((0xdeadbeef << 31) + 0xdeafdeed))
	  {
	    fprintf (stderr, "gmp: (realloc) data clobbered after allocation block\n");
	    abort ();
	  }
      oldptr = p - 1;
    }

  new_size += 2 * BYTES_PER_MP_LIMB;
#endif

  ret = realloc (oldptr, new_size);
  if (ret == 0)
    {
#if defined _MSC_VER && defined _WIN64
        fprintf (stderr, "GNU MP: Cannot reallocate memory (old_size=%llu new_size=%llu)\n", old_size, new_size);
#else
        fprintf (stderr, "GNU MP: Cannot reallocate memory (old_size=%lu new_size=%lu)\n", old_size, new_size);
#endif
      abort ();
    }

#ifdef DEBUG
  {
    mp_ptr p = ret;
    p++;
    p[-1] = (0xdeadbeef << 31) + 0xdeafdeed;
    if (req_size % BYTES_PER_MP_LIMB == 0)
      p[req_size / BYTES_PER_MP_LIMB] = ~((0xdeadbeef << 31) + 0xdeafdeed);
    ret = p;
  }
#endif
  return ret;
}

void
__gmp_default_free (void *blk_ptr, size_t blk_size)
{
#ifdef DEBUG
  {
    mp_ptr p = blk_ptr;
    if (blk_size != 0)
      {
	if (p[-1] != (0xdeadbeef << 31) + 0xdeafdeed)
	  {
	    fprintf (stderr, "gmp: (free) data clobbered before allocation block\n");
	    abort ();
	  }
	if (blk_size % BYTES_PER_MP_LIMB == 0)
	  if (p[blk_size / BYTES_PER_MP_LIMB] != ~((0xdeadbeef << 31) + 0xdeafdeed))
	    {
	      fprintf (stderr, "gmp: (free) data clobbered after allocation block\n");
	      abort ();
	    }
      }
    blk_ptr = p - 1;
  }
#endif
  free (blk_ptr);
}


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

