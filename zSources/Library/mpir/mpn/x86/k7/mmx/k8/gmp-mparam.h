//////////////////////////////////////////////////////////////////////
// gmp-mparam.h
/* Generated by tuneup.c, 2010-03-24, gcc 4.4 */

#define MUL_KARATSUBA_THRESHOLD          28
#define MUL_TOOM3_THRESHOLD             129
#define MUL_TOOM4_THRESHOLD             214
#define MUL_TOOM8H_THRESHOLD            288

#define SQR_BASECASE_THRESHOLD            0  /* always (native) */
#define SQR_KARATSUBA_THRESHOLD          48
#define SQR_TOOM3_THRESHOLD              90
#define SQR_TOOM4_THRESHOLD             232
#define SQR_TOOM8_THRESHOLD             262

#define POWM_THRESHOLD                  180

#define GCDEXT_THRESHOLD                 39
#define JACOBI_BASE_METHOD                1

#define USE_PREINV_DIVREM_1               1  /* native */
#define USE_PREINV_MOD_1                  1  /* native */
#define DIVREM_2_THRESHOLD                0  /* always */
#define DIVEXACT_1_THRESHOLD              0  /* always (native) */
#define MODEXACT_1_ODD_THRESHOLD          0  /* always (native) */
#define MOD_1_1_THRESHOLD                59
#define MOD_1_2_THRESHOLD                60
#define MOD_1_3_THRESHOLD               306
#define DIVREM_HENSEL_QR_1_THRESHOLD     28
#define RSH_DIVREM_HENSEL_QR_1_THRESHOLD     10
#define DIVREM_EUCLID_HENSEL_THRESHOLD      9

#define ROOTREM_THRESHOLD                 6

#define GET_STR_DC_THRESHOLD             14
#define GET_STR_PRECOMPUTE_THRESHOLD     27
#define SET_STR_DC_THRESHOLD           9970
#define SET_STR_PRECOMPUTE_THRESHOLD  16019

#define MUL_FFT_FULL_THRESHOLD         1664

#define SQR_FFT_FULL_THRESHOLD         1664

#define MULLOW_BASECASE_THRESHOLD         7
#define MULLOW_DC_THRESHOLD              17
#define MULLOW_MUL_THRESHOLD           2852

#define MULHIGH_BASECASE_THRESHOLD       24
#define MULHIGH_DC_THRESHOLD             24
#define MULHIGH_MUL_THRESHOLD          2852

#define MULMOD_2EXPM1_THRESHOLD          22

#define FAC_UI_THRESHOLD              32756
#define DC_DIV_QR_THRESHOLD             104
#define DC_DIVAPPR_Q_N_THRESHOLD        998
#define INV_DIV_QR_THRESHOLD           3344
#define INV_DIVAPPR_Q_N_THRESHOLD       998
#define DC_DIV_Q_THRESHOLD              998
#define INV_DIV_Q_THRESHOLD            1017
#define DC_DIVAPPR_Q_THRESHOLD          998
#define INV_DIVAPPR_Q_THRESHOLD        3119
#define DC_BDIV_QR_THRESHOLD            102
#define DC_BDIV_Q_THRESHOLD             960

/* fft_tuning -- autogenerated by tune-fft */

#define FFT_TAB \
   { { 4, 3 }, { 3, 2 }, { 2, 1 }, { 2, 1 }, { 1, 0 } }

#define MULMOD_TAB \
   { 4, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 1, 1 }

#define FFT_N_NUM 15

#define FFT_MULMOD_2EXPP1_CUTOFF 128

/* Tuneup completed successfully, took 378 seconds */


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

