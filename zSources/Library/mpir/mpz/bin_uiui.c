//////////////////////////////////////////////////////////////////////
// bin_uiui.c
/* mpz_bin_uiui - compute n over k.

Contributed to the GNU project by Torbjorn Granlund and Marco Bodrato.

Copyright 2010, 2011, 2012 Free Software Foundation, Inc.

This file is part of the GNU MP Library.

The GNU MP Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 3 of the License, or (at your
option) any later version.

The GNU MP Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the GNU MP Library.  If not, see http://www.gnu.org/licenses/.  */

#include "mpir.h"
#include "gmp-impl.h"
#include "longlong.h"

#ifndef BIN_GOETGHELUCK_THRESHOLD
#define BIN_GOETGHELUCK_THRESHOLD  1000
#endif
#ifndef BIN_UIUI_ENABLE_SMALLDC
#define BIN_UIUI_ENABLE_SMALLDC    1
#endif
#ifndef BIN_UIUI_RECURSIVE_SMALLDC
#define BIN_UIUI_RECURSIVE_SMALLDC (GMP_NUMB_BITS > 32)
#endif

#if GMP_LIMB_BITS == 64

/* This table contains inverses of odd factorials, modulo 2^GMP_NUMB_BITS */

/* It begins with (2!/2)^-1=1 */
#define ONE_LIMB_ODD_FACTORIAL_INVERSES_TABLE CNST_LIMB(0x1),CNST_LIMB(0xaaaaaaaaaaaaaaab),CNST_LIMB(0xaaaaaaaaaaaaaaab),CNST_LIMB(0xeeeeeeeeeeeeeeef),CNST_LIMB(0x4fa4fa4fa4fa4fa5),CNST_LIMB(0x2ff2ff2ff2ff2ff3),CNST_LIMB(0x2ff2ff2ff2ff2ff3),CNST_LIMB(0x938cc70553e3771b),CNST_LIMB(0xb71c27cddd93e49f),CNST_LIMB(0xb38e3229fcdee63d),CNST_LIMB(0xe684bb63544a4cbf),CNST_LIMB(0xc2f684917ca340fb),CNST_LIMB(0xf747c9cba417526d),CNST_LIMB(0xbb26eb51d7bd49c3),CNST_LIMB(0xbb26eb51d7bd49c3),CNST_LIMB(0xb0a7efb985294093),CNST_LIMB(0xbe4b8c69f259eabb),CNST_LIMB(0x6854d17ed6dc4fb9),CNST_LIMB(0xe1aa904c915f4325),CNST_LIMB(0x3b8206df131cead1),CNST_LIMB(0x79c6009fea76fe13),CNST_LIMB(0xd8c5d381633cd365),CNST_LIMB(0x4841f12b21144677),CNST_LIMB(0x4a91ff68200b0d0f),CNST_LIMB(0x8f9513a58c4f9e8b),CNST_LIMB(0x2b3e690621a42251),CNST_LIMB(0x4f520f00e03c04e7),CNST_LIMB(0x2edf84ee600211d3),CNST_LIMB(0xadcaa2764aaacdfd),CNST_LIMB(0x161f4f9033f4fe63),CNST_LIMB(0x161f4f9033f4fe63),CNST_LIMB(0xbada2932ea4d3e03),CNST_LIMB(0xcec189f3efaa30d3),CNST_LIMB(0xf7475bb68330bf91),CNST_LIMB(0x37eb7bf7d5b01549),CNST_LIMB(0x46b35660a4e91555),CNST_LIMB(0xa567c12d81f151f7),CNST_LIMB(0x4c724007bb2071b1),CNST_LIMB(0xf4a0cce58a016bd),CNST_LIMB(0xfa21068e66106475),CNST_LIMB(0x244ab72b5a318ae1),CNST_LIMB(0x366ce67e080d0f23),CNST_LIMB(0xd666fdae5dd2a449),CNST_LIMB(0xd740ddd0acc06a0d),CNST_LIMB(0xb050bbbb28e6f97b),CNST_LIMB(0x70b003fe890a5c75),CNST_LIMB(0xd03aabff83037427),CNST_LIMB(0x13ec4ca72c783bd7),CNST_LIMB(0x90282c06afdbd96f),CNST_LIMB(0x4414ddb9db4a95d5),CNST_LIMB(0xa2c68735ae6832e9),CNST_LIMB(0xbf72d71455676665),CNST_LIMB(0xa8469fab6b759b7f),CNST_LIMB(0xc1e55b56e606caf9),CNST_LIMB(0x40455630fc4a1cff),CNST_LIMB(0x120a7b0046d16f7),CNST_LIMB(0xa7c3553b08faef23),CNST_LIMB(0x9f0bfd1b08d48639),CNST_LIMB(0xa433ffce9a304d37),CNST_LIMB(0xa22ad1d53915c683),CNST_LIMB(0xcb6cbc723ba5dd1d),CNST_LIMB(0x547fb1b8ab9d0ba3),CNST_LIMB(0x547fb1b8ab9d0ba3),CNST_LIMB(0x8f15a826498852e3)

#define ODD_FACTORIAL_TABLE_MAX CNST_LIMB(0x335281867ec241ef)
#define ODD_FACTORIAL_TABLE_LIMIT (25)

#define ODD_FACTORIAL_EXTTABLE_LIMIT (67)

#define ODD_CENTRAL_BINOMIAL_OFFSET (13)

/* This table contains binomial(2k,k)/2^t */

/* It begins with ODD_CENTRAL_BINOMIAL_TABLE_MIN */
#define ONE_LIMB_ODD_CENTRAL_BINOMIAL_TABLE CNST_LIMB(0x13d66b),CNST_LIMB(0x4c842f),CNST_LIMB(0x93ee7d),CNST_LIMB(0x11e9e123),CNST_LIMB(0x22c60053),CNST_LIMB(0x873ae4d1),CNST_LIMB(0x10757bd97),CNST_LIMB(0x80612c6cd),CNST_LIMB(0xfaa556bc1),CNST_LIMB(0x3d3cc24821),CNST_LIMB(0x77cfeb6bbb),CNST_LIMB(0x7550ebd97c7),CNST_LIMB(0xe5f08695caf),CNST_LIMB(0x386120ffce11),CNST_LIMB(0x6eabb28dd6df),CNST_LIMB(0x3658e31c82a8f),CNST_LIMB(0x6ad2050312783),CNST_LIMB(0x1a42902a5af0bf),CNST_LIMB(0x33ac44f881661d),CNST_LIMB(0xcb764f927d82123),CNST_LIMB(0x190c23fa46b93983),CNST_LIMB(0x62b7609e25caf1b9),CNST_LIMB(0xc29cb72925ef2cff)
#define ODD_CENTRAL_BINOMIAL_TABLE_LIMIT (35)

/* This table contains the inverses of elements in the previous table. */
#define ONE_LIMB_ODD_CENTRAL_BINOMIAL_INVERSE_TABLE CNST_LIMB(0x61e5bd199bb12643),CNST_LIMB(0x78321494dc8342cf),CNST_LIMB(0x4fd348704ebf7ad5),CNST_LIMB(0x7e722ba086ab568b),CNST_LIMB(0xa5fcc124265843db),CNST_LIMB(0x89c4a6b18633f431),CNST_LIMB(0x4daa2c15f8ce9227),CNST_LIMB(0x801c618ca9be9605),CNST_LIMB(0x32dc192f948a441),CNST_LIMB(0xd02b90c2bf3be1),CNST_LIMB(0xd897e8c1749aa173),CNST_LIMB(0x54a234fc01fef9f7),CNST_LIMB(0x83ff2ab4d1ff7a4f),CNST_LIMB(0xa427f1c9b304e2f1),CNST_LIMB(0x9c14595d1793651f),CNST_LIMB(0x883a71c607a7b46f),CNST_LIMB(0xd089863c54bc9f2b),CNST_LIMB(0x9022f6bce5d07f3f),CNST_LIMB(0xbec207e218768c35),CNST_LIMB(0x9d70cb4cbb4f168b),CNST_LIMB(0x3c3d3403828a9d2b),CNST_LIMB(0x7672df58c56bc489),CNST_LIMB(0x1e66ca55d727d2ff)

/* This table contains the values t in the formula binomial(2k,k)/2^t */
#define CENTRAL_BINOMIAL_2FAC_TABLE 3,3,4,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,1,2,2,3

#else

/* This table contains inverses of odd factorials, modulo 2^GMP_NUMB_BITS */

/* It begins with (2!/2)^-1=1 */
#define ONE_LIMB_ODD_FACTORIAL_INVERSES_TABLE CNST_LIMB(0x1),CNST_LIMB(0xaaaaaaab),CNST_LIMB(0xaaaaaaab),CNST_LIMB(0xeeeeeeef),CNST_LIMB(0xa4fa4fa5),CNST_LIMB(0xf2ff2ff3),CNST_LIMB(0xf2ff2ff3),CNST_LIMB(0x53e3771b),CNST_LIMB(0xdd93e49f),CNST_LIMB(0xfcdee63d),CNST_LIMB(0x544a4cbf),CNST_LIMB(0x7ca340fb),CNST_LIMB(0xa417526d),CNST_LIMB(0xd7bd49c3),CNST_LIMB(0xd7bd49c3),CNST_LIMB(0x85294093),CNST_LIMB(0xf259eabb),CNST_LIMB(0xd6dc4fb9),CNST_LIMB(0x915f4325),CNST_LIMB(0x131cead1),CNST_LIMB(0xea76fe13),CNST_LIMB(0x633cd365),CNST_LIMB(0x21144677),CNST_LIMB(0x200b0d0f),CNST_LIMB(0x8c4f9e8b),CNST_LIMB(0x21a42251),CNST_LIMB(0xe03c04e7),CNST_LIMB(0x600211d3),CNST_LIMB(0x4aaacdfd),CNST_LIMB(0x33f4fe63),CNST_LIMB(0x33f4fe63)

#define ODD_FACTORIAL_TABLE_MAX CNST_LIMB(0x260eeeeb)
#define ODD_FACTORIAL_TABLE_LIMIT (16)

#define ODD_FACTORIAL_EXTTABLE_LIMIT (34)

#define ODD_CENTRAL_BINOMIAL_OFFSET (8)

/* This table contains binomial(2k,k)/2^t */

/* It begins with ODD_CENTRAL_BINOMIAL_TABLE_MIN */
#define ONE_LIMB_ODD_CENTRAL_BINOMIAL_TABLE CNST_LIMB(0x1923),CNST_LIMB(0x2f7b),CNST_LIMB(0xb46d),CNST_LIMB(0x15873),CNST_LIMB(0xa50c7),CNST_LIMB(0x13d66b),CNST_LIMB(0x4c842f),CNST_LIMB(0x93ee7d),CNST_LIMB(0x11e9e123),CNST_LIMB(0x22c60053),CNST_LIMB(0x873ae4d1)
#define ODD_CENTRAL_BINOMIAL_TABLE_LIMIT (18)

/* This table contains the inverses of elements in the previous table. */
#define ONE_LIMB_ODD_CENTRAL_BINOMIAL_INVERSE_TABLE CNST_LIMB(0x16a2de8b),CNST_LIMB(0x847457b3),CNST_LIMB(0xfa6f7565),CNST_LIMB(0xf0e50cbb),CNST_LIMB(0xdca370f7),CNST_LIMB(0x9bb12643),CNST_LIMB(0xdc8342cf),CNST_LIMB(0x4ebf7ad5),CNST_LIMB(0x86ab568b),CNST_LIMB(0x265843db),CNST_LIMB(0x8633f431)

/* This table contains the values t in the formula binomial(2k,k)/2^t */
#define CENTRAL_BINOMIAL_2FAC_TABLE 1,2,2,3,2,3,3,4,1,2,2

#endif

/* Algorithm:

   Accumulate chunks of factors first limb-by-limb (using one of mul0-mul8)
   which are then accumulated into mpn numbers.  The first inner loop
   accumulates divisor factors, the 2nd inner loop accumulates exactly the same
   number of dividend factors.  We avoid accumulating more for the divisor,
   even with its smaller factors, since we else cannot guarantee divisibility.

   Since we know each division will yield an integer, we compute the quotient
   using Hensel norm: If the quotient is limited by 2^t, we compute A / B mod
   2^t.

   Improvements:

   (1) An obvious improvement to this code would be to compute mod 2^t
   everywhere.  Unfortunately, we cannot determine t beforehand, unless we
   invoke some approximation, such as Stirling's formula.  Of course, we don't
   need t to be tight.  However, it is not clear that this would help much,
   our numbers are kept reasonably small already.

   (2) Compute nmax/kmax semi-accurately, without scalar division or a loop.
   Extracting the 3 msb, then doing a table lookup using cnt*8+msb as index,
   would make it both reasonably accurate and fast.  (We could use a table
   stored into a limb, perhaps.)  The table should take the removed factors of
   2 into account (those done on-the-fly in mulN).

   (3) The first time in the loop we compute the odd part of a
   factorial in kp, we might use oddfac_1 for this task.
 */

/* This threshold determines how large divisor to accumulate before we call
   bdiv.  Perhaps we should never call bdiv, and accumulate all we are told,
   since we are just basecase code anyway?  Presumably, this depends on the
   relative speed of the asymptotically fast code and this code.  */
#define SOME_THRESHOLD 20

/* Multiply-into-limb functions.  These remove factors of 2 on-the-fly.  FIXME:
   All versions of MAXFACS don't take this 2 removal into account now, meaning
   that then, shifting just adds some overhead.  (We remove factors from the
   completed limb anyway.)  */

static mp_limb_t
mul1 (mp_limb_t m)
{
  return m;
}

static mp_limb_t
mul2 (mp_limb_t m)
{
  /* We need to shift before multiplying, to avoid an overflow. */
  mp_limb_t m01 = (m | 1) * ((m + 1) >> 1);
  return m01;
}

static mp_limb_t
mul3 (mp_limb_t m)
{
  mp_limb_t m01 = (m + 0) * (m + 1) >> 1;
  mp_limb_t m2 = (m + 2);
  return m01 * m2;
}

static mp_limb_t
mul4 (mp_limb_t m)
{
  mp_limb_t m01 = (m + 0) * (m + 1) >> 1;
  mp_limb_t m23 = (m + 2) * (m + 3) >> 1;
  return m01 * m23;
}

static mp_limb_t
mul5 (mp_limb_t m)
{
  mp_limb_t m012 = (m + 0) * (m + 1) * (m + 2) >> 1;
  mp_limb_t m34 = (m + 3) * (m + 4) >> 1;
  return m012 * m34;
}

static mp_limb_t
mul6 (mp_limb_t m)
{
  mp_limb_t m01 = (m + 0) * (m + 1);
  mp_limb_t m23 = (m + 2) * (m + 3);
  mp_limb_t m45 = (m + 4) * (m + 5) >> 1;
  mp_limb_t m0123 = m01 * m23 >> 3;
  return m0123 * m45;
}

static mp_limb_t
mul7 (mp_limb_t m)
{
  mp_limb_t m01 = (m + 0) * (m + 1);
  mp_limb_t m23 = (m + 2) * (m + 3);
  mp_limb_t m456 = (m + 4) * (m + 5) * (m + 6) >> 1;
  mp_limb_t m0123 = m01 * m23 >> 3;
  return m0123 * m456;
}

static mp_limb_t
mul8 (mp_limb_t m)
{
  mp_limb_t m01 = (m + 0) * (m + 1);
  mp_limb_t m23 = (m + 2) * (m + 3);
  mp_limb_t m45 = (m + 4) * (m + 5);
  mp_limb_t m67 = (m + 6) * (m + 7);
  mp_limb_t m0123 = m01 * m23 >> 3;
  mp_limb_t m4567 = m45 * m67 >> 3;
  return m0123 * m4567;
}

typedef mp_limb_t (* mulfunc_t) (mp_limb_t);

static const mulfunc_t mulfunc[] = {mul1,mul2,mul3,mul4,mul5,mul6,mul7,mul8};
#define M (numberof(mulfunc))

/* Number of factors-of-2 removed by the corresponding mulN functon.  */
static const unsigned char tcnttab[] = {0, 1, 1, 2, 2, 4, 4, 6};

#if 1
/* This variant is inaccurate but share the code with other functions.  */
#define MAXFACS(max,l)							\
  do {									\
    (max) = log_n_max (l);						\
  } while (0)
#else

/* This variant is exact(?) but uses a loop.  It takes the 2 removal
 of mulN into account.  */
static const unsigned long ftab[] =
#if GMP_NUMB_BITS == 64
  /* 1 to 8 factors per iteration */
  {CNST_LIMB(0xffffffffffffffff),CNST_LIMB(0x100000000),0x32cbfe,0x16a0b,0x24c4,0xa16,0x34b,0x1b2 /*,0xdf,0x8d */};
#endif
#if GMP_NUMB_BITS == 32
  /* 1 to 7 factors per iteration */
  {0xffffffff,0x10000,0x801,0x16b,0x71,0x42,0x26 /* ,0x1e */};
#endif

#define MAXFACS(max,l)							\
  do {									\
    int __i;								\
    for (__i = numberof (ftab) - 1; l > ftab[__i]; __i--)		\
      ;									\
    (max) = __i + 1;							\
  } while (0)
#endif

/* Entry i contains (i!/2^t)^(-1) where t is chosen such that the parenthesis
   is an odd integer. */
static const mp_limb_t facinv[] = { ONE_LIMB_ODD_FACTORIAL_INVERSES_TABLE };

static void
mpz_bdiv_bin_uiui (mpz_ptr r, mpir_ui n, mpir_ui k)
{
  int nmax, kmax, nmaxnow, numfac;
  mp_ptr np, kp;
  mp_size_t nn, kn, alloc;
  mp_limb_t i, j, t, iii, jjj, cy, dinv;
  mp_bitcnt_t i2cnt, j2cnt;
  int cnt;
  mp_size_t maxn;
  TMP_DECL;

  ASSERT (k > ODD_FACTORIAL_TABLE_LIMIT);
  TMP_MARK;

  maxn = 1 + n / GMP_NUMB_BITS;    /* absolutely largest result size (limbs) */

  /* FIXME: This allocation might be insufficient, but is usually way too
     large.  */
  alloc = SOME_THRESHOLD - 1 + MAX (3 * maxn / 2, SOME_THRESHOLD);
  alloc = MIN (alloc, k) + 1;
  np = TMP_ALLOC_LIMBS (alloc);
  kp = TMP_ALLOC_LIMBS (SOME_THRESHOLD + 1);

  MAXFACS (nmax, n);
  ASSERT (nmax <= M);
  MAXFACS (kmax, k);
  ASSERT (kmax <= M);
  ASSERT (k >= M);

  i = n - k + 1;

  np[0] = 1; nn = 1;

  i2cnt = 0;				/* total low zeros in dividend */
  j2cnt = __gmp_fac2cnt_table[ODD_FACTORIAL_TABLE_LIMIT / 2 - 1];
					/* total low zeros in divisor */

  numfac = 1;
  j = ODD_FACTORIAL_TABLE_LIMIT + 1;
  jjj = ODD_FACTORIAL_TABLE_MAX;
  ASSERT (__gmp_oddfac_table[ODD_FACTORIAL_TABLE_LIMIT] == ODD_FACTORIAL_TABLE_MAX);

  while (1)
    {
      kp[0] = jjj;				/* store new factors */
      kn = 1;
      t = k - j + 1;
      kmax = MIN (kmax, t);

      while (kmax != 0 && kn < SOME_THRESHOLD)
	{
	  jjj = mulfunc[kmax - 1] (j);
	  j += kmax;				/* number of factors used */
	  count_trailing_zeros (cnt, jjj);	/* count low zeros */
	  jjj >>= cnt;				/* remove remaining low zeros */
	  j2cnt += tcnttab[kmax - 1] + cnt;	/* update low zeros count */
	  cy = mpn_mul_1 (kp, kp, kn, jjj);	/* accumulate new factors */
	  kp[kn] = cy;
	  kn += cy != 0;
	  t = k - j + 1;
	  kmax = MIN (kmax, t);
	}
      numfac = j - numfac;

      while (numfac != 0)
	{
	  nmaxnow = MIN (nmax, numfac);
	  iii = mulfunc[nmaxnow - 1] (i);
	  i += nmaxnow;				/* number of factors used */
	  count_trailing_zeros (cnt, iii);	/* count low zeros */
	  iii >>= cnt;				/* remove remaining low zeros */
	  i2cnt += tcnttab[nmaxnow - 1] + cnt;	/* update low zeros count */
	  cy = mpn_mul_1 (np, np, nn, iii);	/* accumulate new factors */
	  np[nn] = cy;
	  nn += cy != 0;
	  numfac -= nmaxnow;
	}

      ASSERT (nn < alloc);

      modlimb_invert (dinv, kp[0]);
      nn += (np[nn - 1] >= kp[kn - 1]);
      nn -= kn;
      {
         mp_limb_t wp[2];
         
         mpn_sb_bdiv_q (np, wp, np, nn, kp, MIN(kn,nn), dinv);
      }
      if (kmax == 0)
	break;
      numfac = j;

      jjj = mulfunc[kmax - 1] (j);
      j += kmax;				/* number of factors used */
      count_trailing_zeros (cnt, jjj);		/* count low zeros */
      jjj >>= cnt;				/* remove remaining low zeros */
      j2cnt += tcnttab[kmax - 1] + cnt;		/* update low zeros count */
    }

  /* Put back the right number of factors of 2.  */
  cnt = i2cnt - j2cnt;
  if (cnt != 0)
    {
      ASSERT (cnt < GMP_NUMB_BITS); /* can happen, but not for intended use */
      cy = mpn_lshift (np, np, nn, cnt);
      np[nn] = cy;
      nn += cy != 0;
    }

  nn -= np[nn - 1] == 0;	/* normalisation */

  kp = MPZ_NEWALLOC (r, nn);
  SIZ(r) = nn;
  MPN_COPY (kp, np, nn);
  TMP_FREE;
}

static void
mpz_smallk_bin_uiui (mpz_ptr r, mpir_ui n, mpir_ui k)
{
  int nmax, numfac;
  mp_ptr rp;
  mp_size_t rn, alloc;
  mp_limb_t i, iii, cy;
  mp_bitcnt_t i2cnt, cnt;

  count_leading_zeros (cnt, (mp_limb_t) n);
  cnt = GMP_LIMB_BITS - cnt;
  alloc = cnt * k / GMP_NUMB_BITS + 3;	/* FIXME: ensure rounding is enough. */
  rp = MPZ_NEWALLOC (r, alloc);

  MAXFACS (nmax, n);
  nmax = MIN (nmax, M);

  i = n - k + 1;

  nmax = MIN (nmax, k);
  rp[0] = mulfunc[nmax - 1] (i);
  rn = 1;
  i += nmax;				/* number of factors used */
  i2cnt = tcnttab[nmax - 1];		/* low zeros count */
  numfac = k - nmax;
  while (numfac != 0)
    {
      nmax = MIN (nmax, numfac);
      iii = mulfunc[nmax - 1] (i);
      i += nmax;			/* number of factors used */
      i2cnt += tcnttab[nmax - 1];	/* update low zeros count */
      cy = mpn_mul_1 (rp, rp, rn, iii);	/* accumulate new factors */
      rp[rn] = cy;
      rn += cy != 0;
      numfac -= nmax;
    }

  ASSERT (rn < alloc);

  mpn_divrem_hensel_rsh_qr_1_preinv (rp, rp, rn, __gmp_oddfac_table[k],
		    facinv[k - 2], __gmp_fac2cnt_table[k / 2 - 1] - i2cnt); 
  /* A two-fold, branch-free normalisation is possible :*/
  /* rn -= rp[rn - 1] == 0; */
  /* rn -= rp[rn - 1] == 0; */
  MPN_NORMALIZE_NOT_ZERO (rp, rn);

  SIZ(r) = rn;
}

/* Algorithm:

   Plain and simply multiply things together.

   We tabulate factorials (k!/2^t)^(-1) mod B (where t is chosen such
   that k!/2^t is odd).

*/

static mp_limb_t
bc_bin_uiui (mpir_ui n, mpir_ui k)
{
  return ((__gmp_oddfac_table[n] * facinv[k - 2] * facinv[n - k - 2])
    << (__gmp_fac2cnt_table[n / 2 - 1] - __gmp_fac2cnt_table[k / 2 - 1] - __gmp_fac2cnt_table[(n-k) / 2 - 1]))
    & GMP_NUMB_MASK;
}

/* Algorithm:

   Recursively exploit the relation
   bin(n,k) = bin(n,k>>1)*bin(n-k>>1,k-k>>1)/bin(k,k>>1) .

   Values for binomial(k,k>>1) that fit in a limb are precomputed
   (with inverses).
*/

/* bin2kk[i - ODD_CENTRAL_BINOMIAL_OFFSET] =
   binomial(i*2,i)/2^t (where t is chosen so that it is odd). */
static const mp_limb_t bin2kk[] = { ONE_LIMB_ODD_CENTRAL_BINOMIAL_TABLE };

/* bin2kkinv[i] = bin2kk[i]^-1 mod B */
static const mp_limb_t bin2kkinv[] = { ONE_LIMB_ODD_CENTRAL_BINOMIAL_INVERSE_TABLE };

/* bin2kk[i] = binomial((i+MIN_S)*2,i+MIN_S)/2^t. This table contains the t values. */
static const unsigned char fac2bin[] = { CENTRAL_BINOMIAL_2FAC_TABLE };

static void
mpz_smallkdc_bin_uiui (mpz_ptr r, mpir_ui n, mpir_ui k)
{
  mp_ptr rp;
  mp_size_t rn;
  mpir_ui hk;

  hk = k >> 1;

  if ((! BIN_UIUI_RECURSIVE_SMALLDC) || hk <= ODD_FACTORIAL_TABLE_LIMIT)
    mpz_smallk_bin_uiui (r, n, hk);
  else
    mpz_smallkdc_bin_uiui (r, n, hk);
  k -= hk;
  n -= hk;
  if (n <= ODD_FACTORIAL_EXTTABLE_LIMIT) {
    mp_limb_t cy;
    rn = SIZ (r);
    rp = MPZ_REALLOC (r, rn + 1);
    cy = mpn_mul_1 (rp, rp, rn, bc_bin_uiui (n, k));
    rp [rn] = cy;
    rn += cy != 0;
  } else {
    mp_limb_t buffer[ODD_CENTRAL_BINOMIAL_TABLE_LIMIT + 3];
    mpz_t t;

    ALLOC (t) = ODD_CENTRAL_BINOMIAL_TABLE_LIMIT + 3;
    PTR (t) = buffer;
    if ((! BIN_UIUI_RECURSIVE_SMALLDC) || k <= ODD_FACTORIAL_TABLE_LIMIT)
      mpz_smallk_bin_uiui (t, n, k);
    else
      mpz_smallkdc_bin_uiui (t, n, k);
    mpz_mul (r, r, t);
    rp = PTR (r);
    rn = SIZ (r);
  }

  mpn_divrem_hensel_rsh_qr_1_preinv (rp, rp, rn, bin2kk[k - ODD_CENTRAL_BINOMIAL_OFFSET],
		   bin2kkinv[k - ODD_CENTRAL_BINOMIAL_OFFSET],
		    fac2bin[k - ODD_CENTRAL_BINOMIAL_OFFSET] - (k != hk));
  /* A two-fold, branch-free normalisation is possible :*/
  /* rn -= rp[rn - 1] == 0; */
  /* rn -= rp[rn - 1] == 0; */
  MPN_NORMALIZE_NOT_ZERO (rp, rn);

  SIZ(r) = rn;
}

/* mpz_goetgheluck_bin_uiui(RESULT, N, K) -- Set RESULT to binomial(N,K).
 *
 * Contributed to the GNU project by Marco Bodrato.
 *
 * Implementation of the algorithm by P. Goetgheluck, "Computing
 * Binomial Coefficients", The American Mathematical Monthly, Vol. 94,
 * No. 4 (April 1987), pp. 360-365.
 *
 * Acknowledgment: Peter Luschny did spot the slowness of the previous
 * code and suggested the reference.
 */

/* TODO: Remove duplicated constants / macros / static functions...
 */

/*************************************************************/
/* Section macros: common macros, for swing/fac/bin (&sieve) */
/*************************************************************/

#define FACTOR_LIST_APPEND(PR, MAX_PR, VEC, I)			\
  if ((PR) > (MAX_PR)) {					\
    (VEC)[(I)++] = (PR);					\
    (PR) = 1;							\
  }

#define FACTOR_LIST_STORE(P, PR, MAX_PR, VEC, I)		\
  do {								\
    if ((PR) > (MAX_PR)) {					\
      (VEC)[(I)++] = (PR);					\
      (PR) = (P);						\
    } else							\
      (PR) *= (P);						\
  } while (0)

#define LOOP_ON_SIEVE_CONTINUE(prime,end,sieve)			\
    __max_i = (end);						\
								\
    do {							\
      ++__i;							\
      if (((sieve)[__index] & __mask) == 0)			\
	{							\
	  (prime) = id_to_n(__i)

#define LOOP_ON_SIEVE_BEGIN(prime,start,end,off,sieve)		\
  do {								\
    mp_limb_t __mask, __index, __max_i, __i;			\
								\
    __i = (start)-(off);					\
    __index = __i / GMP_LIMB_BITS;				\
    __mask = CNST_LIMB(1) << (__i % GMP_LIMB_BITS);		\
    __i += (off);						\
								\
    LOOP_ON_SIEVE_CONTINUE(prime,end,sieve)

#define LOOP_ON_SIEVE_STOP					\
	}							\
      __mask = __mask << 1 | __mask >> (GMP_LIMB_BITS-1);	\
      __index += __mask & 1;					\
    }  while (__i <= __max_i)					\

#define LOOP_ON_SIEVE_END					\
    LOOP_ON_SIEVE_STOP;						\
  } while (0)

/*********************************************************/
/* Section sieve: sieving functions and tools for primes */
/*********************************************************/

#if WANT_ASSERT
static mp_limb_t
bit_to_n (mp_limb_t bit) { return (bit*3+4)|1; }
#endif

/* id_to_n (x) = bit_to_n (x-1) = (id*3+1)|1*/
static mp_limb_t
id_to_n  (mp_limb_t id)  { return id*3+1+(id&1); }

/* n_to_bit (n) = ((n-1)&(-CNST_LIMB(2)))/3U-1 */
static mp_limb_t
n_to_bit (mp_limb_t n) { return ((n-5)|1)/3U; }

static mp_size_t
primesieve_size (mp_limb_t n) { return n_to_bit(n) / GMP_LIMB_BITS + 1; }

/*********************************************************/
/* Section binomial: fast binomial implementation        */
/*********************************************************/

#define COUNT_A_PRIME(P, N, K, PR, MAX_PR, VEC, I)	\
  do {							\
    mp_limb_t __a, __b, __prime, __ma,__mb;		\
    __prime = (P);					\
    __a = (N); __b = (K); __mb = 0;			\
    FACTOR_LIST_APPEND(PR, MAX_PR, VEC, I);		\
    do {						\
      __mb += __b % __prime; __b /= __prime;		\
      __ma = __a % __prime; __a /= __prime;		\
      if (__ma < __mb) {				\
        __mb = 1; (PR) *= __prime;			\
      } else  __mb = 0;					\
    } while (__a >= __prime);				\
  } while (0)

#define SH_COUNT_A_PRIME(P, N, K, PR, MAX_PR, VEC, I)	\
  do {							\
    mp_limb_t __prime;					\
    __prime = (P);					\
    if (((N) % __prime) < ((K) % __prime)) {		\
      FACTOR_LIST_STORE (__prime, PR, MAX_PR, VEC, I);	\
    }							\
  } while (0)

/* Returns an approximation of the sqare root of x.  *
 * It gives: x <= limb_apprsqrt (x) ^ 2 < x * 9/4    */
static mp_limb_t
limb_apprsqrt (mp_limb_t x)
{
  int s;

  ASSERT (x > 2);
  count_leading_zeros (s, x - 1);
  s = GMP_LIMB_BITS - 1 - s;
  return (CNST_LIMB(1) << (s >> 1)) + (CNST_LIMB(1) << ((s - 1) >> 1));
}

static void
mpz_goetgheluck_bin_uiui (mpz_ptr r, unsigned long int n, unsigned long int k)
{
  mp_limb_t *sieve, *factors, count;
  mp_limb_t prod, max_prod, j;
  TMP_DECL;

  ASSERT (BIN_GOETGHELUCK_THRESHOLD >= 13);
  ASSERT (n >= 25);

  TMP_MARK;
  sieve = TMP_ALLOC_LIMBS (primesieve_size (n));

  count = gmp_primesieve (sieve, n) + 1;
  factors = TMP_ALLOC_LIMBS (count / log_n_max (n) + 1);

  max_prod = GMP_NUMB_MAX / n;

  /* Handle primes = 2, 3 separately. */
  popc_limb (count, n - k);
  popc_limb (j, k);
  count += j;
  popc_limb (j, n);
  count -= j;
  prod = CNST_LIMB(1) << count;

  j = 0;
  COUNT_A_PRIME (3, n, k, prod, max_prod, factors, j);

  /* Accumulate prime factors from 5 to n/2 */
    {
      mp_limb_t s;

      {
	mp_limb_t prime;
	s = limb_apprsqrt(n);
	s = n_to_bit (s);
	LOOP_ON_SIEVE_BEGIN (prime, n_to_bit (5), s, 0,sieve);
	COUNT_A_PRIME (prime, n, k, prod, max_prod, factors, j);
	LOOP_ON_SIEVE_END;
	s++;
      }

      ASSERT (max_prod <= GMP_NUMB_MAX / 2);
      max_prod <<= 1;
      ASSERT (bit_to_n (s) * bit_to_n (s) > n);
      ASSERT (s <= n_to_bit (n >> 1));
      {
	mp_limb_t prime;

	LOOP_ON_SIEVE_BEGIN (prime, s, n_to_bit (n >> 1), 0,sieve);
	SH_COUNT_A_PRIME (prime, n, k, prod, max_prod, factors, j);
	LOOP_ON_SIEVE_END;
      }
      max_prod >>= 1;
    }

  /* Store primes from (n-k)+1 to n */
  ASSERT (n_to_bit (n - k) < n_to_bit (n));
    {
      mp_limb_t prime;
      LOOP_ON_SIEVE_BEGIN (prime, n_to_bit (n - k) + 1, n_to_bit (n), 0,sieve);
      FACTOR_LIST_STORE (prime, prod, max_prod, factors, j);
      LOOP_ON_SIEVE_END;
    }

  if (LIKELY (j != 0))
    {
      factors[j++] = prod;
      mpz_prodlimbs (r, factors, j);
    }
  else
    {
      PTR (r)[0] = prod;
      SIZ (r) = 1;
    }
  TMP_FREE;
}

#undef COUNT_A_PRIME
#undef SH_COUNT_A_PRIME
#undef LOOP_ON_SIEVE_END
#undef LOOP_ON_SIEVE_STOP
#undef LOOP_ON_SIEVE_BEGIN
#undef LOOP_ON_SIEVE_CONTINUE

/*********************************************************/
/* End of implementation of Goetgheluck's algorithm      */
/*********************************************************/

void
mpz_bin_uiui (mpz_ptr r, mpir_ui n, mpir_ui k)
{
  if (UNLIKELY (n < k)) {
    SIZ (r) = 0;
#if BITS_PER_ULONG > GMP_NUMB_BITS
  } else if (UNLIKELY (n > GMP_NUMB_MAX)) {
    mpz_t tmp;

    mpz_init_set_ui (tmp, n);
    mpz_bin_ui (r, tmp, k);
    mpz_clear (tmp);
#endif
  } else {
    ASSERT (n <= GMP_NUMB_MAX);
    /* Rewrite bin(n,k) as bin(n,n-k) if that is smaller. */
    k = MIN (k, n - k);
    if (k < 2) {
      PTR(r)[0] = k ? n : 1; /* 1 + ((-k) & (n-1)); */
      SIZ(r) = 1;
    } else if (n <= ODD_FACTORIAL_EXTTABLE_LIMIT) { /* k >= 2, n >= 4 */
      PTR(r)[0] = bc_bin_uiui (n, k);
      SIZ(r) = 1;
    } else if (k <= ODD_FACTORIAL_TABLE_LIMIT)
      mpz_smallk_bin_uiui (r, n, k);
    else if (BIN_UIUI_ENABLE_SMALLDC &&
	     k <= (BIN_UIUI_RECURSIVE_SMALLDC ? ODD_CENTRAL_BINOMIAL_TABLE_LIMIT : ODD_FACTORIAL_TABLE_LIMIT)* 2)
      mpz_smallkdc_bin_uiui (r, n, k);
    else if (ABOVE_THRESHOLD (k, BIN_GOETGHELUCK_THRESHOLD) &&
	     k > (n >> 4)) /* k > ODD_FACTORIAL_TABLE_LIMIT */
      mpz_goetgheluck_bin_uiui (r, n, k);
    else
      mpz_bdiv_bin_uiui (r, n, k);
  }
}


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

