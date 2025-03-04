//////////////////////////////////////////////////////////////////////
// gmp-mparam.h
/* Generated by tuneup.c, 2014-03-21, gcc 4.4 */

#define MUL_KARATSUBA_THRESHOLD          16
#define MUL_TOOM3_THRESHOLD              89
#define MUL_TOOM4_THRESHOLD             166
#define MUL_TOOM8H_THRESHOLD            270

#define SQR_BASECASE_THRESHOLD            0  /* always (native) */
#define SQR_KARATSUBA_THRESHOLD          24
#define SQR_TOOM3_THRESHOLD              93
#define SQR_TOOM4_THRESHOLD             262
#define SQR_TOOM8_THRESHOLD             351

#define POWM_THRESHOLD                  736

#define DIVREM_1_NORM_THRESHOLD       MP_SIZE_T_MAX  /* never */
#define DIVREM_1_UNNORM_THRESHOLD     MP_SIZE_T_MAX  /* never */
#define MOD_1_NORM_THRESHOLD              0  /* always */
#define MOD_1_UNNORM_THRESHOLD            0  /* always */
#define USE_PREINV_DIVREM_1               1  /* native */
#define USE_PREINV_MOD_1                  1
#define DIVEXACT_1_THRESHOLD              0  /* always */
#define MODEXACT_1_ODD_THRESHOLD          0  /* always (native) */
#define MOD_1_1_THRESHOLD                 5
#define MOD_1_2_THRESHOLD                 8
#define MOD_1_3_THRESHOLD                19
#define DIVREM_HENSEL_QR_1_THRESHOLD     12
#define RSH_DIVREM_HENSEL_QR_1_THRESHOLD      7
#define DIVREM_EUCLID_HENSEL_THRESHOLD     52

#define MUL_FFT_FULL_THRESHOLD         3008

#define SQR_FFT_FULL_THRESHOLD         2368

#define MULLOW_BASECASE_THRESHOLD         6
#define MULLOW_DC_THRESHOLD              11
#define MULLOW_MUL_THRESHOLD           4525

#define MULHIGH_BASECASE_THRESHOLD       16
#define MULHIGH_DC_THRESHOLD             16
#define MULHIGH_MUL_THRESHOLD          2587

#define MULMOD_2EXPM1_THRESHOLD          14

#define DC_DIV_QR_THRESHOLD              38
#define INV_DIV_QR_THRESHOLD           1895
#define INV_DIVAPPR_Q_N_THRESHOLD        38
#define DC_DIV_Q_THRESHOLD               43
#define INV_DIV_Q_THRESHOLD            1078
#define DC_DIVAPPR_Q_THRESHOLD           36
#define INV_DIVAPPR_Q_THRESHOLD       15352
#define DC_BDIV_QR_THRESHOLD             36
#define DC_BDIV_Q_THRESHOLD              11

#define ROOTREM_THRESHOLD                 6

#define MATRIX22_STRASSEN_THRESHOLD      13
#define HGCD_THRESHOLD                   85
#define HGCD_APPR_THRESHOLD              50
#define HGCD_REDUCE_THRESHOLD          6852
#define GCD_DC_THRESHOLD                379
#define GCDEXT_DC_THRESHOLD             278
#define JACOBI_BASE_METHOD                1

#define GET_STR_DC_THRESHOLD             17
#define GET_STR_PRECOMPUTE_THRESHOLD     21
#define SET_STR_DC_THRESHOLD            933
#define SET_STR_PRECOMPUTE_THRESHOLD   1638

#define FAC_DSC_THRESHOLD               486
#define FAC_ODD_THRESHOLD                42

/* fft_tuning -- autogenerated by tune-fft */

#define FFT_TAB \
   { { 4, 3 }, { 3, 3 }, { 2, 2 }, { 2, 1 }, { 1, 0 } }

#define MULMOD_TAB \
   { 4, 4, 3, 4, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 1, 2, 2, 1, 1 }

#define FFT_N_NUM 19

#define FFT_MULMOD_2EXPP1_CUTOFF 128


/* Tuneup completed successfully, took 110 seconds */



//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

