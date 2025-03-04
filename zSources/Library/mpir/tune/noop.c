//////////////////////////////////////////////////////////////////////
// noop.c
/* Noop routines.

   These are in a separate file to stop gcc recognising do-nothing functions
   and optimizing away calls to them.  */

/*
Copyright 1999, 2000 Free Software Foundation, Inc.

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

#include "mpir.h"
#include "gmp-impl.h"

#include "speed.h"


void
noop (void)
{
}

/*ARGSUSED*/
void
noop_1 (mp_limb_t n)
{
}

/*ARGSUSED*/
void
noop_wxs (mp_ptr wp, mp_srcptr xp, mp_size_t size)
{
}

/*ARGSUSED*/
void
noop_wxys (mp_ptr wp, mp_srcptr xp, mp_srcptr yp, mp_size_t size)
{
}

/*ARGSUSED*/
void 
mpn_cache_fill_dummy (mp_limb_t n)
{
}


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

