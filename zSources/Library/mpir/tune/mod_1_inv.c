//////////////////////////////////////////////////////////////////////
// mod_1_inv.c
/* mpn/generic/mod_1.c forced to use mul-by-inverse udiv_qrnnd_preinv.

Copyright 2000 Free Software Foundation, Inc.

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

#define OPERATION_mod_1

#include "mpir.h"
#include "gmp-impl.h"

#undef MOD_1_NORM_THRESHOLD
#undef MOD_1_UNNORM_THRESHOLD
#define MOD_1_NORM_THRESHOLD    0
#define MOD_1_UNNORM_THRESHOLD  0
#define __gmpn_mod_1  mpn_mod_1_inv

#include "mpn/generic/mod_1.c"


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

