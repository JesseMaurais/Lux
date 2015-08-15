#ifndef CBLAS_H
#define CBLAS_H
#include <stddef.h>
#include <complex>

/* For easier switching between implementations of the complex numbers */
template <class real> using complex = std::complex<real>;

/* Allow the use in C++ code.  */
#ifdef __cplusplus
extern "C" 
{
#endif

/*
 * ===========================================================================
 * Prototypes for level 1 BLAS functions (complex are recast as routines)
 * ===========================================================================
 */
float  cblas_sdsdot(const int N, const float alpha, const float *X,
                    const int incX, const float *Y, const int incY);
double cblas_dsdot(const int N, const float *X, const int incX, const float *Y,
                   const int incY);
float  cblas_sdot(const int N, const float  *X, const int incX,
                  const float  *Y, const int incY);
double cblas_ddot(const int N, const double *X, const int incX,
                  const double *Y, const int incY);

/*
 * Functions having prefixes Z and C only
 */
void   cblas_cdotu_sub(const int N, const complex<float> *X, const int incX,
                       const complex<float> *Y, const int incY, complex<float> *dotu);
void   cblas_cdotc_sub(const int N, const complex<float> *X, const int incX,
                       const complex<float> *Y, const int incY, complex<float> *dotc);

void   cblas_zdotu_sub(const int N, const complex<double> *X, const int incX,
                       const complex<double> *Y, const int incY, complex<double> *dotu);
void   cblas_zdotc_sub(const int N, const complex<double> *X, const int incX,
                       const complex<double> *Y, const int incY, complex<double> *dotc);


/*
 * Functions having prefixes S D SC DZ
 */
float  cblas_snrm2(const int N, const float *X, const int incX);
float  cblas_sasum(const int N, const float *X, const int incX);

double cblas_dnrm2(const int N, const double *X, const int incX);
double cblas_dasum(const int N, const double *X, const int incX);

float  cblas_scnrm2(const int N, const complex<float> *X, const int incX);
float  cblas_scasum(const int N, const complex<float> *X, const int incX);

double cblas_dznrm2(const int N, const complex<double> *X, const int incX);
double cblas_dzasum(const int N, const complex<double> *X, const int incX);


/*
 * Functions having standard 4 prefixes (S D C Z)
 */
size_t cblas_isamax(const int N, const float  *X, const int incX);
size_t cblas_idamax(const int N, const double *X, const int incX);
size_t cblas_icamax(const int N, const complex<float> *X, const int incX);
size_t cblas_izamax(const int N, const complex<double> *X, const int incX);

/*
 * ===========================================================================
 * Prototypes for level 1 BLAS routines
 * ===========================================================================
 */

/* 
 * Routines with standard 4 prefixes (s, d, c, z)
 */
void cblas_sswap(const int N, float *X, const int incX, 
                 float *Y, const int incY);
void cblas_scopy(const int N, const float *X, const int incX, 
                 float *Y, const int incY);
void cblas_saxpy(const int N, const float alpha, const float *X,
                 const int incX, float *Y, const int incY);

void cblas_dswap(const int N, double *X, const int incX, 
                 double *Y, const int incY);
void cblas_dcopy(const int N, const double *X, const int incX, 
                 double *Y, const int incY);
void cblas_daxpy(const int N, const double alpha, const double *X,
                 const int incX, double *Y, const int incY);

void cblas_cswap(const int N, complex<float> *X, const int incX, 
                 complex<float> *Y, const int incY);
void cblas_ccopy(const int N, const complex<float> *X, const int incX, 
                 complex<float> *Y, const int incY);
void cblas_caxpy(const int N, const complex<float> *alpha, const complex<float> *X,
                 const int incX, complex<float> *Y, const int incY);

void cblas_zswap(const int N, complex<double> *X, const int incX, 
                 float *Y, const int incY);
void cblas_zcopy(const int N, const complex<double> *X, const int incX, 
                 float *Y, const int incY);
void cblas_zaxpy(const int N, const complex<double> *alpha, const complex<double> *X,
                 const int incX, complex<double> *Y, const int incY);


/* 
 * Routines with S and D prefix only
 */
void cblas_srotg(float *a, float *b, float *c, float *s);
void cblas_srotmg(float *d1, float *d2, float *b1, const float b2, float *P);
void cblas_srot(const int N, float *X, const int incX,
                float *Y, const int incY, const float c, const float s);
void cblas_srotm(const int N, float *X, const int incX,
                float *Y, const int incY, const float *P);

void cblas_drotg(double *a, double *b, double *c, double *s);
void cblas_drotmg(double *d1, double *d2, double *b1, const double b2, double *P);
void cblas_drot(const int N, double *X, const int incX,
                double *Y, const int incY, const double c, const double  s);
void cblas_drotm(const int N, double *X, const int incX,
                double *Y, const int incY, const double *P);


/* 
 * Routines with S D C Z CS and ZD prefixes
 */
void cblas_sscal(const int N, const float alpha, float *X, const int incX);
void cblas_dscal(const int N, const double alpha, double *X, const int incX);
void cblas_cscal(const int N, const complex<float> *alpha, complex<float> *X, const int incX);
void cblas_zscal(const int N, const complex<double> *alpha, complex<double> *X, const int incX);
void cblas_csscal(const int N, const float alpha, complex<float> *X, const int incX);
void cblas_zdscal(const int N, const double alpha, complex<double> *X, const int incX);

/*
 * ===========================================================================
 * Prototypes for level 2 BLAS
 * ===========================================================================
 */

/* 
 * Routines with standard 4 prefixes (S, D, C, Z)
 */
void cblas_sgemv(const char order,
                 const char TransA, const int M, const int N,
                 const float alpha, const float *A, const int lda,
                 const float *X, const int incX, const float beta,
                 float *Y, const int incY);
void cblas_sgbmv(const char order,
                 const char TransA, const int M, const int N,
                 const int KL, const int KU, const float alpha,
                 const float *A, const int lda, const float *X,
                 const int incX, const float beta, float *Y, const int incY);
void cblas_strmv(const char order, const char Uplo,
                 const char TransA, const char Diag,
                 const int N, const float *A, const int lda, 
                 float *X, const int incX);
void cblas_stbmv(const char order, const char Uplo,
                 const char TransA, const char Diag,
                 const int N, const int K, const float *A, const int lda, 
                 float *X, const int incX);
void cblas_stpmv(const char order, const char Uplo,
                 const char TransA, const char Diag,
                 const int N, const float *Ap, float *X, const int incX);
void cblas_strsv(const char order, const char Uplo,
                 const char TransA, const char Diag,
                 const int N, const float *A, const int lda, float *X,
                 const int incX);
void cblas_stbsv(const char order, const char Uplo,
                 const char TransA, const char Diag,
                 const int N, const int K, const float *A, const int lda,
                 float *X, const int incX);
void cblas_stpsv(const char order, const char Uplo,
                 const char TransA, const char Diag,
                 const int N, const float *Ap, float *X, const int incX);

void cblas_dgemv(const char order,
                 const char TransA, const int M, const int N,
                 const double alpha, const double *A, const int lda,
                 const double *X, const int incX, const double beta,
                 double *Y, const int incY);
void cblas_dgbmv(const char order,
                 const char TransA, const int M, const int N,
                 const int KL, const int KU, const double alpha,
                 const double *A, const int lda, const double *X,
                 const int incX, const double beta, double *Y, const int incY);
void cblas_dtrmv(const char order, const char Uplo,
                 const char TransA, const char Diag,
                 const int N, const double *A, const int lda, 
                 double *X, const int incX);
void cblas_dtbmv(const char order, const char Uplo,
                 const char TransA, const char Diag,
                 const int N, const int K, const double *A, const int lda, 
                 double *X, const int incX);
void cblas_dtpmv(const char order, const char Uplo,
                 const char TransA, const char Diag,
                 const int N, const double *Ap, double *X, const int incX);
void cblas_dtrsv(const char order, const char Uplo,
                 const char TransA, const char Diag,
                 const int N, const double *A, const int lda, double *X,
                 const int incX);
void cblas_dtbsv(const char order, const char Uplo,
                 const char TransA, const char Diag,
                 const int N, const int K, const double *A, const int lda,
                 double *X, const int incX);
void cblas_dtpsv(const char order, const char Uplo,
                 const char TransA, const char Diag,
                 const int N, const double *Ap, double *X, const int incX);

void cblas_cgemv(const char order,
                 const char TransA, const int M, const int N,
                 const complex<float> *alpha, const complex<float> *A, const int lda,
                 const complex<float> *X, const int incX, const complex<float> *beta,
                 complex<float> *Y, const int incY);
void cblas_cgbmv(const char order,
                 const char TransA, const int M, const int N,
                 const int KL, const int KU, const complex<float> *alpha,
                 const complex<float> *A, const int lda, const complex<float> *X,
                 const int incX, const complex<float> *beta, complex<float> *Y, const int incY);
void cblas_ctrmv(const char order, const char Uplo,
                 const char TransA, const char Diag,
                 const int N, const complex<float> *A, const int lda, 
                 complex<float> *X, const int incX);
void cblas_ctbmv(const char order, const char Uplo,
                 const char TransA, const char Diag,
                 const int N, const int K, const complex<float> *A, const int lda, 
                 complex<float> *X, const int incX);
void cblas_ctpmv(const char order, const char Uplo,
                 const char TransA, const char Diag,
                 const int N, const complex<float> *Ap, complex<float> *X, const int incX);
void cblas_ctrsv(const char order, const char Uplo,
                 const char TransA, const char Diag,
                 const int N, const complex<float> *A, const int lda, complex<float> *X,
                 const int incX);
void cblas_ctbsv(const char order, const char Uplo,
                 const char TransA, const char Diag,
                 const int N, const int K, const complex<float> *A, const int lda,
                 complex<float> *X, const int incX);
void cblas_ctpsv(const char order, const char Uplo,
                 const char TransA, const char Diag,
                 const int N, const complex<float> *Ap, complex<float> *X, const int incX);

void cblas_zgemv(const char order,
                 const char TransA, const int M, const int N,
                 const complex<double> *alpha, const complex<double> *A, const int lda,
                 const complex<double> *X, const int incX, const complex<double> *beta,
                 complex<double> *Y, const int incY);
void cblas_zgbmv(const char order,
                 const char TransA, const int M, const int N,
                 const int KL, const int KU, const complex<double> *alpha,
                 const complex<double> *A, const int lda, const complex<double> *X,
                 const int incX, const complex<double> *beta, complex<double> *Y, const int incY);
void cblas_ztrmv(const char order, const char Uplo,
                 const char TransA, const char Diag,
                 const int N, const complex<double> *A, const int lda, 
                 complex<double> *X, const int incX);
void cblas_ztbmv(const char order, const char Uplo,
                 const char TransA, const char Diag,
                 const int N, const int K, const complex<double> *A, const int lda, 
                 complex<double> *X, const int incX);
void cblas_ztpmv(const char order, const char Uplo,
                 const char TransA, const char Diag,
                 const int N, const complex<double> *Ap, complex<double> *X, const int incX);
void cblas_ztrsv(const char order, const char Uplo,
                 const char TransA, const char Diag,
                 const int N, const complex<double> *A, const int lda, complex<double> *X,
                 const int incX);
void cblas_ztbsv(const char order, const char Uplo,
                 const char TransA, const char Diag,
                 const int N, const int K, const complex<double> *A, const int lda,
                 complex<double> *X, const int incX);
void cblas_ztpsv(const char order, const char Uplo,
                 const char TransA, const char Diag,
                 const int N, const complex<double> *Ap, complex<double> *X, const int incX);


/* 
 * Routines with S and D prefixes only
 */
void cblas_ssymv(const char order, const char Uplo,
                 const int N, const float alpha, const float *A,
                 const int lda, const float *X, const int incX,
                 const float beta, float *Y, const int incY);
void cblas_ssbmv(const char order, const char Uplo,
                 const int N, const int K, const float alpha, const float *A,
                 const int lda, const float *X, const int incX,
                 const float beta, float *Y, const int incY);
void cblas_sspmv(const char order, const char Uplo,
                 const int N, const float alpha, const float *Ap,
                 const float *X, const int incX,
                 const float beta, float *Y, const int incY);
void cblas_sger(const char order, const int M, const int N,
                const float alpha, const float *X, const int incX,
                const float *Y, const int incY, float *A, const int lda);
void cblas_ssyr(const char order, const char Uplo,
                const int N, const float alpha, const float *X,
                const int incX, float *A, const int lda);
void cblas_sspr(const char order, const char Uplo,
                const int N, const float alpha, const float *X,
                const int incX, float *Ap);
void cblas_ssyr2(const char order, const char Uplo,
                const int N, const float alpha, const float *X,
                const int incX, const float *Y, const int incY, float *A,
                const int lda);
void cblas_sspr2(const char order, const char Uplo,
                const int N, const float alpha, const float *X,
                const int incX, const float *Y, const int incY, float *A);

void cblas_dsymv(const char order, const char Uplo,
                 const int N, const double alpha, const double *A,
                 const int lda, const double *X, const int incX,
                 const double beta, double *Y, const int incY);
void cblas_dsbmv(const char order, const char Uplo,
                 const int N, const int K, const double alpha, const double *A,
                 const int lda, const double *X, const int incX,
                 const double beta, double *Y, const int incY);
void cblas_dspmv(const char order, const char Uplo,
                 const int N, const double alpha, const double *Ap,
                 const double *X, const int incX,
                 const double beta, double *Y, const int incY);
void cblas_dger(const char order, const int M, const int N,
                const double alpha, const double *X, const int incX,
                const double *Y, const int incY, double *A, const int lda);
void cblas_dsyr(const char order, const char Uplo,
                const int N, const double alpha, const double *X,
                const int incX, double *A, const int lda);
void cblas_dspr(const char order, const char Uplo,
                const int N, const double alpha, const double *X,
                const int incX, double *Ap);
void cblas_dsyr2(const char order, const char Uplo,
                const int N, const double alpha, const double *X,
                const int incX, const double *Y, const int incY, double *A,
                const int lda);
void cblas_dspr2(const char order, const char Uplo,
                const int N, const double alpha, const double *X,
                const int incX, const double *Y, const int incY, double *A);


/* 
 * Routines with C and Z prefixes only
 */
void cblas_chemv(const char order, const char Uplo,
                 const int N, const complex<float> *alpha, const complex<float> *A,
                 const int lda, const complex<float> *X, const int incX,
                 const complex<float> *beta, complex<float> *Y, const int incY);
void cblas_chbmv(const char order, const char Uplo,
                 const int N, const int K, const complex<float> *alpha, const complex<float> *A,
                 const int lda, const complex<float> *X, const int incX,
                 const complex<float> *beta, complex<float> *Y, const int incY);
void cblas_chpmv(const char order, const char Uplo,
                 const int N, const complex<float> *alpha, const complex<float> *Ap,
                 const complex<float> *X, const int incX,
                 const complex<float> *beta, complex<float> *Y, const int incY);
void cblas_cgeru(const char order, const int M, const int N,
                 const complex<float> *alpha, const complex<float> *X, const int incX,
                 const complex<float> *Y, const int incY, complex<float> *A, const int lda);
void cblas_cgerc(const char order, const int M, const int N,
                 const complex<float> *alpha, const complex<float> *X, const int incX,
                 const complex<float> *Y, const int incY, complex<float> *A, const int lda);
void cblas_cher(const char order, const char Uplo,
                const int N, const float alpha, const complex<float> *X, const int incX,
                complex<float> *A, const int lda);
void cblas_chpr(const char order, const char Uplo,
                const int N, const float alpha, const complex<float> *X,
                const int incX, complex<float> *A);
void cblas_cher2(const char order, const char Uplo, const int N,
                const complex<float> *alpha, const complex<float> *X, const int incX,
                const complex<float> *Y, const int incY, complex<float> *A, const int lda);
void cblas_chpr2(const char order, const char Uplo, const int N,
                const complex<float> *alpha, const complex<float> *X, const int incX,
                const complex<float> *Y, const int incY, complex<float> *Ap);

void cblas_zhemv(const char order, const char Uplo,
                 const int N, const complex<double> *alpha, const complex<double> *A,
                 const int lda, const complex<double> *X, const int incX,
                 const complex<double> *beta, complex<double> *Y, const int incY);
void cblas_zhbmv(const char order, const char Uplo,
                 const int N, const int K, const complex<double> *alpha, const complex<double> *A,
                 const int lda, const complex<double> *X, const int incX,
                 const complex<double> *beta, complex<double> *Y, const int incY);
void cblas_zhpmv(const char order, const char Uplo,
                 const int N, const complex<double> *alpha, const complex<double> *Ap,
                 const complex<double> *X, const int incX,
                 const complex<double> *beta, complex<double> *Y, const int incY);
void cblas_zgeru(const char order, const int M, const int N,
                 const complex<double> *alpha, const complex<double> *X, const int incX,
                 const complex<double> *Y, const int incY, complex<double> *A, const int lda);
void cblas_zgerc(const char order, const int M, const int N,
                 const complex<double> *alpha, const complex<double> *X, const int incX,
                 const complex<double> *Y, const int incY, complex<double> *A, const int lda);
void cblas_zher(const char order, const char Uplo,
                const int N, const double alpha, const complex<double> *X, const int incX,
                complex<double> *A, const int lda);
void cblas_zhpr(const char order, const char Uplo,
                const int N, const double alpha, const complex<double> *X,
                const int incX, complex<double> *A);
void cblas_zher2(const char order, const char Uplo, const int N,
                const complex<double> *alpha, const complex<double> *X, const int incX,
                const complex<double> *Y, const int incY, complex<double> *A, const int lda);
void cblas_zhpr2(const char order, const char Uplo, const int N,
                const complex<double> *alpha, const complex<double> *X, const int incX,
                const complex<double> *Y, const int incY, complex<double> *Ap);

/*
 * ===========================================================================
 * Prototypes for level 3 BLAS
 * ===========================================================================
 */

/* 
 * Routines with standard 4 prefixes (S, D, C, Z)
 */
void cblas_sgemm(const char Order, const char TransA,
                 const char TransB, const int M, const int N,
                 const int K, const float alpha, const float *A,
                 const int lda, const float *B, const int ldb,
                 const float beta, float *C, const int ldc);
void cblas_ssymm(const char Order, const char Side,
                 const char Uplo, const int M, const int N,
                 const float alpha, const float *A, const int lda,
                 const float *B, const int ldb, const float beta,
                 float *C, const int ldc);
void cblas_ssyrk(const char Order, const char Uplo,
                 const char Trans, const int N, const int K,
                 const float alpha, const float *A, const int lda,
                 const float beta, float *C, const int ldc);
void cblas_ssyr2k(const char Order, const char Uplo,
                  const char Trans, const int N, const int K,
                  const float alpha, const float *A, const int lda,
                  const float *B, const int ldb, const float beta,
                  float *C, const int ldc);
void cblas_strmm(const char Order, const char Side,
                 const char Uplo, const char TransA,
                 const char Diag, const int M, const int N,
                 const float alpha, const float *A, const int lda,
                 float *B, const int ldb);
void cblas_strsm(const char Order, const char Side,
                 const char Uplo, const char TransA,
                 const char Diag, const int M, const int N,
                 const float alpha, const float *A, const int lda,
                 float *B, const int ldb);

void cblas_dgemm(const char Order, const char TransA,
                 const char TransB, const int M, const int N,
                 const int K, const double alpha, const double *A,
                 const int lda, const double *B, const int ldb,
                 const double beta, double *C, const int ldc);
void cblas_dsymm(const char Order, const char Side,
                 const char Uplo, const int M, const int N,
                 const double alpha, const double *A, const int lda,
                 const double *B, const int ldb, const double beta,
                 double *C, const int ldc);
void cblas_dsyrk(const char Order, const char Uplo,
                 const char Trans, const int N, const int K,
                 const double alpha, const double *A, const int lda,
                 const double beta, double *C, const int ldc);
void cblas_dsyr2k(const char Order, const char Uplo,
                  const char Trans, const int N, const int K,
                  const double alpha, const double *A, const int lda,
                  const double *B, const int ldb, const double beta,
                  double *C, const int ldc);
void cblas_dtrmm(const char Order, const char Side,
                 const char Uplo, const char TransA,
                 const char Diag, const int M, const int N,
                 const double alpha, const double *A, const int lda,
                 double *B, const int ldb);
void cblas_dtrsm(const char Order, const char Side,
                 const char Uplo, const char TransA,
                 const char Diag, const int M, const int N,
                 const double alpha, const double *A, const int lda,
                 double *B, const int ldb);

void cblas_cgemm(const char Order, const char TransA,
                 const char TransB, const int M, const int N,
                 const int K, const complex<float> *alpha, const complex<float> *A,
                 const int lda, const complex<float> *B, const int ldb,
                 const complex<float> *beta, complex<float> *C, const int ldc);
void cblas_csymm(const char Order, const char Side,
                 const char Uplo, const int M, const int N,
                 const complex<float> *alpha, const complex<float> *A, const int lda,
                 const complex<float> *B, const int ldb, const complex<float> *beta,
                 complex<float> *C, const int ldc);
void cblas_csyrk(const char Order, const char Uplo,
                 const char Trans, const int N, const int K,
                 const complex<float> *alpha, const complex<float> *A, const int lda,
                 const complex<float> *beta, complex<float> *C, const int ldc);
void cblas_csyr2k(const char Order, const char Uplo,
                  const char Trans, const int N, const int K,
                  const complex<float> *alpha, const complex<float> *A, const int lda,
                  const complex<float> *B, const int ldb, const complex<float> *beta,
                  complex<float> *C, const int ldc);
void cblas_ctrmm(const char Order, const char Side,
                 const char Uplo, const char TransA,
                 const char Diag, const int M, const int N,
                 const complex<float> *alpha, const complex<float> *A, const int lda,
                 complex<float> *B, const int ldb);
void cblas_ctrsm(const char Order, const char Side,
                 const char Uplo, const char TransA,
                 const char Diag, const int M, const int N,
                 const complex<float> *alpha, const complex<float> *A, const int lda,
                 complex<float> *B, const int ldb);

void cblas_zgemm(const char Order, const char TransA,
                 const char TransB, const int M, const int N,
                 const int K, const complex<double> *alpha, const complex<double> *A,
                 const int lda, const complex<double> *B, const int ldb,
                 const complex<double> *beta, complex<double> *C, const int ldc);
void cblas_zsymm(const char Order, const char Side,
                 const char Uplo, const int M, const int N,
                 const complex<double> *alpha, const complex<double> *A, const int lda,
                 const complex<double> *B, const int ldb, const complex<double> *beta,
                 complex<double> *C, const int ldc);
void cblas_zsyrk(const char Order, const char Uplo,
                 const char Trans, const int N, const int K,
                 const complex<double> *alpha, const complex<double> *A, const int lda,
                 const complex<double> *beta, complex<double> *C, const int ldc);
void cblas_zsyr2k(const char Order, const char Uplo,
                  const char Trans, const int N, const int K,
                  const complex<double> *alpha, const complex<double> *A, const int lda,
                  const complex<double> *B, const int ldb, const complex<double> *beta,
                  complex<double> *C, const int ldc);
void cblas_ztrmm(const char Order, const char Side,
                 const char Uplo, const char TransA,
                 const char Diag, const int M, const int N,
                 const complex<double> *alpha, const complex<double> *A, const int lda,
                 complex<double> *B, const int ldb);
void cblas_ztrsm(const char Order, const char Side,
                 const char Uplo, const char TransA,
                 const char Diag, const int M, const int N,
                 const complex<double> *alpha, const complex<double> *A, const int lda,
                 complex<double> *B, const int ldb);


/* 
 * Routines with prefixes C and Z only
 */
void cblas_chemm(const char Order, const char Side,
                 const char Uplo, const int M, const int N,
                 const complex<float> *alpha, const complex<float> *A, const int lda,
                 const complex<float> *B, const int ldb, const complex<float> *beta,
                 complex<float> *C, const int ldc);
void cblas_cherk(const char Order, const char Uplo,
                 const char Trans, const int N, const int K,
                 const float alpha, const complex<float> *A, const int lda,
                 const float beta, complex<float> *C, const int ldc);
void cblas_cher2k(const char Order, const char Uplo,
                  const char Trans, const int N, const int K,
                  const complex<float> *alpha, const complex<float> *A, const int lda,
                  const complex<float> *B, const int ldb, const float beta,
                  complex<float> *C, const int ldc);

void cblas_zhemm(const char Order, const char Side,
                 const char Uplo, const int M, const int N,
                 const complex<double> *alpha, const complex<double> *A, const int lda,
                 const complex<double> *B, const int ldb, const complex<double> *beta,
                 complex<double> *C, const int ldc);
void cblas_zherk(const char Order, const char Uplo,
                 const char Trans, const int N, const int K,
                 const double alpha, const complex<double> *A, const int lda,
                 const double beta, complex<double> *C, const int ldc);
void cblas_zher2k(const char Order, const char Uplo,
                  const char Trans, const int N, const int K,
                  const complex<double> *alpha, const complex<double> *A, const int lda,
                  const complex<double> *B, const int ldb, const double beta,
                  complex<double> *C, const int ldc);

void cblas_xerbla(int p, const char *rout, const char *form, ...);

#ifdef __cplusplus
}
#endif 

#endif
