//////////////////////////////////////////////////////////////////////
// fft_negacyclic.c
/* 

Copyright 2009, 2011 William Hart. All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are
permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice, this list of
      conditions and the following disclaimer.

   2. Redistributions in binary form must reproduce the above copyright notice, this list
      of conditions and the following disclaimer in the documentation and/or other materials
      provided with the distribution.

THIS SOFTWARE IS PROVIDED BY William Hart ``AS IS'' AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL William Hart OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those of the
authors and should not be interpreted as representing official policies, either expressed
or implied, of William Hart.

*/

#include "mpir.h"
#include "gmp-impl.h"
      
void mpir_fft_negacyclic(mp_ptr * ii, mp_size_t n, mp_bitcnt_t w, 
                    mp_ptr * t1, mp_ptr * t2, mp_ptr * temp)
{
   mp_size_t i;
   mp_size_t limbs = (w*n)/GMP_LIMB_BITS;
   
   /* first apply twiddle factors corresponding to shifts of w*i/2 bits */
   if (w & 1)
   {
      for (i = 0; i < n; i++) 
      {   
          mpir_fft_adjust(*t1, ii[i], i/2, limbs, w);
          MP_PTR_SWAP(ii[i], *t1);
            
          mpir_fft_adjust(*t2, ii[n+i], (n+i)/2, limbs, w);
          MP_PTR_SWAP(ii[n+i], *t2);

          mpir_fft_butterfly(*t1, *t2, ii[i], ii[n+i], i, limbs, w);
          MP_PTR_SWAP(ii[i],   *t1);
          MP_PTR_SWAP(ii[n+i], *t2);

          i++;
          
          mpir_fft_adjust_sqrt2(*t1, ii[i], i, limbs, w, *temp);
          MP_PTR_SWAP(ii[i], *t1);
          
          mpir_fft_adjust_sqrt2(*t2, ii[n+i], n+i, limbs, w, *temp);
          MP_PTR_SWAP(ii[n+i], *t2);
          
          mpir_fft_butterfly(*t1, *t2, ii[i], ii[n+i], i, limbs, w);
          MP_PTR_SWAP(ii[i],   *t1);
          MP_PTR_SWAP(ii[n+i], *t2);
       }
   } else
   {
       for (i = 0; i < n; i++) 
       {   
          mpir_fft_adjust(*t1, ii[i], i, limbs, w/2);
          MP_PTR_SWAP(ii[i], *t1);
            
          mpir_fft_adjust(*t2, ii[n+i], n+i, limbs, w/2);
          MP_PTR_SWAP(ii[n+i], *t2);
      
          mpir_fft_butterfly(*t1, *t2, ii[i], ii[n+i], i, limbs, w);
          MP_PTR_SWAP(ii[i],   *t1);
          MP_PTR_SWAP(ii[n+i], *t2);
       }
   }

   mpir_fft_radix2(ii, n/2, 2*w, t1, t2);
   mpir_fft_radix2(ii+n, n/2, 2*w, t1, t2);
}


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

