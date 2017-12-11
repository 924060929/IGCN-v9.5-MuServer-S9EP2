//////////////////////////////////////////////////////////////////////
// gmp-mparam.h
/* Generated by tuneup.c, 2014-03-25, gcc 4.7 */

#define MUL_KARATSUBA_THRESHOLD          20
#define MUL_TOOM3_THRESHOLD             121
#define MUL_TOOM4_THRESHOLD             178
#define MUL_TOOM8H_THRESHOLD            294

#define SQR_BASECASE_THRESHOLD            8
#define SQR_KARATSUBA_THRESHOLD          40
#define SQR_TOOM3_THRESHOLD             146
#define SQR_TOOM4_THRESHOLD             384
#define SQR_TOOM8_THRESHOLD             384

#define POWM_THRESHOLD                  984

#define DIVREM_1_NORM_THRESHOLD           0  /* preinv always */
#define DIVREM_1_UNNORM_THRESHOLD         0  /* always */
#define MOD_1_NORM_THRESHOLD              0  /* always */
#define MOD_1_UNNORM_THRESHOLD            0  /* always */
#define USE_PREINV_DIVREM_1               1  /* preinv always */
#define USE_PREINV_MOD_1                  1  /* preinv always */
#define DIVREM_2_THRESHOLD                0  /* preinv always */
#define DIVEXACT_1_THRESHOLD              0  /* always */
#define MODEXACT_1_ODD_THRESHOLD          0  /* always */
#define MOD_1_1_THRESHOLD                 6
#define MOD_1_2_THRESHOLD                40
#define MOD_1_3_THRESHOLD                40
#define DIVREM_HENSEL_QR_1_THRESHOLD      5
#define RSH_DIVREM_HENSEL_QR_1_THRESHOLD     53
#define DIVREM_EUCLID_HENSEL_THRESHOLD    129

#define MUL_FFT_FULL_THRESHOLD         3904

#define SQR_FFT_FULL_THRESHOLD         2880

#define MULLOW_BASECASE_THRESHOLD         0  /* always */
#define MULLOW_DC_THRESHOLD              53
#define MULLOW_MUL_THRESHOLD           5565

#define MULHIGH_BASECASE_THRESHOLD        6
#define MULHIGH_DC_THRESHOLD             50
#define MULHIGH_MUL_THRESHOLD          6327

#define MULMOD_2EXPM1_THRESHOLD          26

#define DC_DIV_QR_THRESHOLD              75
#define INV_DIV_QR_THRESHOLD           2801
#define INV_DIVAPPR_Q_N_THRESHOLD        75
#define DC_DIV_Q_THRESHOLD              106
#define INV_DIV_Q_THRESHOLD            1308
#define DC_DIVAPPR_Q_THRESHOLD          159
#define INV_DIVAPPR_Q_THRESHOLD       10225
#define DC_BDIV_QR_THRESHOLD             76
#define DC_BDIV_Q_THRESHOLD              88

#define ROOTREM_THRESHOLD                 6

#define MATRIX22_STRASSEN_THRESHOLD      15
#define HGCD_THRESHOLD                  144
#define HGCD_APPR_THRESHOLD             222
#define HGCD_REDUCE_THRESHOLD          6852
#define GCD_DC_THRESHOLD                642
#define GCDEXT_DC_THRESHOLD             424
#define JACOBI_BASE_METHOD                2

#define GET_STR_DC_THRESHOLD              9
#define GET_STR_PRECOMPUTE_THRESHOLD     20
#define SET_STR_DC_THRESHOLD            272
#define SET_STR_PRECOMPUTE_THRESHOLD   1616

#define FAC_DSC_THRESHOLD               228
#define FAC_ODD_THRESHOLD                34

/* fft_tuning -- autogenerated by tune-fft */

#define FFT_TAB \
   { { 3, 3 }, { 3, 2 }, { 2, 1 }, { 1, 0 }, { 0, 0 } }

#define MULMOD_TAB \
   { 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1 }

#define FFT_N_NUM 19

#define FFT_MULMOD_2EXPP1_CUTOFF 128


/* Tuneup completed successfully, took 779 seconds */



//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

