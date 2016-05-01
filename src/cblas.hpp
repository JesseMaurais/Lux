#ifndef cblas_hpp
#define cblas_hpp

#include <cblas.h>
#include <complex>


template <class real> using complex = std::complex<real>;


///////////////////////////////// PROTOTYPES //////////////////////////////////


// LEVEL 1

// s, d, c, z
template <class number, class scalar=number>
scalar nrm2(const int N, const number *X, const int incX);
template <class number, class scalar=number>
scalar asum(const int N, const number *X, const int incX);
template <class number>
size_t iamax(const int N, const number *X, const int incX);
template <class number>
void swap(const int N, number *X, const int incX, number *Y, const int incY);
template <class number>
void copy(const int N, const number *X, const int incX, number *Y, const int incY);
template <class number>
void axpy(const int N, const number alpha, const number *X, const int incX, number *Y, const int incY);
template <class number, class scalar>
void scal(const int N, const scalar alpha, number *X, const int incX);
template <class number>
void scal(const int N, const number alpha, number *X, const int incX);
// s, d
template <class number>
number dot(const int N, const number *X, const int incX, const number *Y, const int incY);
template <class number>
void rotg(number *a, number  *b, number *c, number *s);
template <class number>
void rotmg(number *d1, number *d2, number *b1, const number b2, number *P);
template <class number>
void rot(const int N, number *X, const int incX, number *Y, const int incY, const number c, const number s);
template <class number>
void rotm(const int N, number *X, const int incX, number *Y, const int incY, const number *P);
// c, z
template <class number>
number dotu(const int N, const number *X, const int incX, const number *Y, const int incY);
template <class number>
number dotc(const int N, const number *X, const int incX, const number *Y, const int incY);

// LEVEL 2

// s, d, c, z
template <class number>
void gemv(const char order, const char transA, const int M, const int N, const number alpha, const number *A, const int lda, const number *X, const int incX, const number beta, number *Y, const int incY);
template <class number>
void gbmv(const char order, const char transA, const int M, const int N, const int KL, const int KU, const number alpha, const number *A, const int lda, const number *X, const int incX, const number beta, number *Y, const int incY);
template <class number>
void trmv(const char order, const char uplo, const char transA, const char diag, const int N, const number *A, const int lda, number *X, const int incX);
template <class number>
void tbmv(const char order, const char uplo, const char transA, const char diag, const int N, const int K, const number *A, const int lda, number *X, const int incX);
template <class number>
void tpmv(const char order, const char uplo, const char transA, const char diag, const int N, const number *Ap, number *X, const int incX);
template <class number>
void trsv(const char order, const char uplo, const char transA, const char diag, const int N, const number *A, const int lda, number *X, const int incX);
template <class number>
void tbsv(const char order, const char uplo, const char transA, const char diag, const int N, const int K, const number *A, const int lda, number *X, const int incX);
template <class number>
void tpsv(const char order, const char uplo, const char transA, const char diag, const int N, const number *Ap, number *X, const int incX);
// s, d
template <class number>
void symv(const char order, const char uplo, const int N, const number alpha, const number *A, const int lda, const number *X, const int incX, const number beta, number *Y, const int incY);
template <class number>
void sbmv(const char order, const char uplo, const int N, const int K, const number alpha, const number *A, const int lda, const number *X, const int incX, const number beta, number *Y, const int incY);
template <class number>
void spmv(const char order, const char uplo, const int N, const number alpha, const number *Ap, const number *X, const int incX, const number beta, number *Y, const int incY);
template <class number>
void ger(const char order, const int M, const int N, const number alpha, const number *X, const int incX, const number *Y, const int incY, number *A, const int lda);
template <class number>
void syr(const char order, const char uplo, const int N, const number alpha, const number *X, const int incX, number *A, const int lda);
template <class number>
void spr(const char order, const char uplo, const int N, const number alpha, const number *X, const int incX, number *Ap);
template <class number>
void syr2(const char order, const char uplo, const int N, const number alpha, const number *X, const int incX, const number *Y, const int incY, number *A, const int lda);
template <class number>
void spr2(const char order, const char uplo, const int N, const number alpha, const number *X, const int incX, const number *Y, const int incY, number *A);
// c, z
template <class number>
void hemv(const char order, const char uplo, const int N, const number alpha, const number *A, const int lda, const number *X, const int incX, const number beta, number *Y, const int incY);
template <class number>
void hbmv(const char order, const char uplo, const int N, const int K, const number alpha, const number *A, const int lda, const number *X, const int incX, const number beta, number *Y, const int incY);
template <class number>
void hpmv(const char order, const char uplo, const int N, const number alpha, const number *Ap, const number *X, const int incX, const number beta, number *Y, const int incY);
template <class number>
void geru(const char order, const int M, const int N, const number alpha, const number *X, const int incX, const number *Y, const int incY, number *A, const int lda);
template <class number>
void gerc(const char order, const int M, const int N, const number alpha, const number *X, const int incX, const number *Y, const int incY, number *A, const int lda);
template <class scalar, class number=complex<scalar>>
void her(const char order, const char uplo, const int N, const scalar alpha, const number *X, const int incX, number *A, const int lda);
template <class scalar, class number=complex<scalar>>
void hpr(const char order, const char uplo, const int N, const scalar alpha, const number *X, const int incX, number *A);
template <class number>
void her2(const char order, const char uplo, const int N, const number alpha, const number *X, const int incX, const number *Y, const int incY, number *A, const int lda);
template <class number>
void hpr2(const char order, const char uplo, const int N, const number alpha, const number *X, const int incX, const number *Y, const int incY, number *Ap);

// LEVEL 3

// s, d, c, z
template <class number>
void gemm(const char order, const char transA, const char transB, const int M, const int N, const int K, const number alpha, const number *A, const int lda, const number *B, const int ldb, const number beta, number *C, const int ldc);
template <class number>
void symm(const char order, const char side, const char uplo, const int M, const int N, const number alpha, const number *A, const int lda, const number *B, const int ldb, const number beta, number *C, const int ldc);
template <class number>
void syrk(const char order, const char uplo, const char trans, const int N, const int K, const number alpha, const number *A, const int lda, const number beta, number *C, const int ldc);
template <class number>
void syr2k(const char order, const char uplo, const char trans, const int N, const int K, const number alpha, const number *A, const int lda, const number *B, const int ldb, const number beta, number *C, const int ldc);
template <class number>
void trmm(const char order, const char side, const char uplo, const char transA, const char diag, const int M, const int N, const number alpha, const number *A, const int lda, number *B, const int ldb);
template <class number>
void trsm(const char order, const char side, const char uplo, const char transA, const char diag, const int M, const int N, const number alpha, const number *A, const int lda, number *B, const int ldb);
// c, z
template <class number>
void hemm(const char order, const char side, const char uplo, const int M, const int N, const number alpha, const number *A, const int lda, const number *B, const int ldb, const number beta, number *C, const int ldc);
template <class scalar, class number=complex<scalar>>
void herk(const char order, const char uplo, const char trans, const int N, const int K, const scalar alpha, const number *A, const int lda, const scalar beta, number *C, const int ldc);
template <class scalar, class number=complex<scalar>>
void her2k(const char order, const char uplo, const char trans, const int N, const int K, const number alpha, const number *A, const int lda, const number *B, const int ldb, const scalar beta, number *C, const int ldc);


/////////////////////////// SINGLE PRECISION FLOAT ////////////////////////////


// LEVEL 1

template <> inline
float dot(const int N, const float *X, const int incX, const float *Y, const int incY)
{
	return cblas_sdot(N, X, incX, Y, incY);
}

template <> inline
float nrm2(const int N, const float *X, const int incX)
{
	return cblas_snrm2(N, X, incX);
}

template <> inline
float asum(const int N, const float *X, const int incX)
{
	return cblas_sasum(N, X, incX);
}

template <> inline
size_t iamax(const int N, const float *X, const int incX)
{
	return cblas_isamax(N, X, incX);
}

template <> inline
void swap(const int N, float *X, const int incX, float *Y, const int incY)
{
	cblas_sswap(N, X, incX, Y, incY);
}

template <> inline
void copy(const int N, const float *X, const int incX, float *Y, const int incY)
{
	cblas_scopy(N, X, incX, Y, incY);
}

template <> inline
void axpy(const int N, const float alpha, const float *X, const int incX, float *Y, const int incY)
{
	cblas_saxpy(N, alpha, X, incX, Y, incY);
}

template <> inline
void rotg(float *a, float *b, float *c, float *s)
{
	cblas_srotg(a, b, c, s);
}

template <> inline
void rotmg(float *d1, float *d2, float *b1, const float b2, float *P)
{
	cblas_srotmg(d1, d2, b1, b2, P);
}

template <> inline
void rot(const int N, float *X, const int incX, float *Y, const int incY, const float c, const float s)
{
	cblas_srot(N, X, incX, Y, incY, c, s);
}

template <> inline
void rotm(const int N, float *X, const int incX, float *Y, const int incY, const float *P)
{
	cblas_srotm(N, X, incX, Y, incY, P);
}

template <> inline
void scal(const int N, const float alpha, float *X, const int incX)
{
	cblas_sscal(N, alpha, X, incX);
}

// LEVEL 2

template <> inline
void gemv(const char order, const char transA, const int M, const int N, const float alpha, const float *A, const int lda, const float *X, const int incX, const float beta, float *Y, const int incY)
{
	cblas_sgemv((CBLAS_ORDER) order, (CBLAS_TRANSPOSE) transA, M, N, alpha, A, lda, X, incX, beta, Y, incY);
}

template <> inline
void gbmv(const char order, const char transA, const int M, const int N, const int KL, const int KU, const float alpha, const float *A, const int lda, const float *X, const int incX, const float beta, float *Y, const int incY)
{
	cblas_sgbmv((CBLAS_ORDER) order, (CBLAS_TRANSPOSE) transA, M, N, KL, KU, alpha, A, lda, X, incX, beta, Y, incY);
}

template <> inline
void trmv(const char order, const char uplo, const char transA, const char diag, const int N, const float *A, const int lda, float *X, const int incX)
{
	cblas_strmv((CBLAS_ORDER) order, (CBLAS_UPLO) uplo, (CBLAS_TRANSPOSE) transA, (CBLAS_DIAG) diag, N, A, lda, X, incX);
}

template <> inline
void tbmv(const char order, const char uplo, const char transA, const char diag, const int N, const int K, const float *A, const int lda, float *X, const int incX)
{
	cblas_stbmv((CBLAS_ORDER) order, (CBLAS_UPLO) uplo, (CBLAS_TRANSPOSE) transA, (CBLAS_DIAG) diag, N, K, A, lda, X, incX);
}

template <> inline
void tpmv(const char order, const char uplo, const char transA, const char diag, const int N, const float *Ap, float *X, const int incX)
{
	cblas_stpmv((CBLAS_ORDER) order, (CBLAS_UPLO) uplo, (CBLAS_TRANSPOSE) transA, (CBLAS_DIAG) diag, N, Ap, X, incX);
}

template <> inline
void trsv(const char order, const char uplo, const char transA, const char diag, const int N, const float *A, const int lda, float *X, const int incX)
{
	cblas_strsv((CBLAS_ORDER) order, (CBLAS_UPLO) uplo, (CBLAS_TRANSPOSE) transA, (CBLAS_DIAG) diag, N, A, lda, X, incX);
}

template <> inline
void tbsv(const char order, const char uplo, const char transA, const char diag, const int N, const int K, const float *A, const int lda, float *X, const int incX)
{
	cblas_stbsv((CBLAS_ORDER) order, (CBLAS_UPLO) uplo, (CBLAS_TRANSPOSE) transA, (CBLAS_DIAG) diag, N, K, A, lda, X, incX);
}

template <> inline
void tpsv(const char order, const char uplo, const char transA, const char diag, const int N, const float *A, float *X, const int incX)
{
	cblas_stpsv((CBLAS_ORDER) order, (CBLAS_UPLO) uplo, (CBLAS_TRANSPOSE) transA, (CBLAS_DIAG) diag, N, A, X, incX);
}

template <> inline
void symv(const char order, const char uplo, const int N, const float alpha, const float *A, const int lda, const float *X, const int incX, const float beta, float *Y, const int incY)
{
	cblas_ssymv((CBLAS_ORDER) order, (CBLAS_UPLO) uplo, N, alpha, A, lda, X, incX, beta, Y, incY);
}

template <> inline
void sbmv(const char order, const char uplo, const int N, const int K, const float alpha, const float *A, const int lda, const float *X, const int incX, const float beta, float *Y, const int incY)
{
	cblas_ssbmv((CBLAS_ORDER) order, (CBLAS_UPLO) uplo, N, K, alpha, A, lda, X, incX, beta, Y, incY);
}

template <> inline
void spmv(const char order, const char uplo, const int N, const float alpha, const float *Ap, const float *X, const int incX, const float beta, float *Y, const int incY)
{
	cblas_sspmv((CBLAS_ORDER) order, (CBLAS_UPLO) uplo, N, alpha, Ap, X, incX, beta, Y, incY);
}

template <> inline
void ger(const char order, const int M, const int N, const float alpha, const float *X, const int incX, const float *Y, const int incY, float *A, const int lda)
{
	cblas_sger((CBLAS_ORDER) order, M, N, alpha, X, incX, Y, incY, A, lda);
}

template <> inline
void syr(const char order, const char uplo, const int N, const float alpha, const float *X, const int incX, float *A, const int lda)
{
	cblas_ssyr((CBLAS_ORDER) order, (CBLAS_UPLO) uplo, N, alpha, X, incX, A, lda);
}

template <> inline
void spr(const char order, const char uplo, const int N, const float alpha, const float *X, const int incX, float *Ap)
{
	cblas_sspr((CBLAS_ORDER) order, (CBLAS_UPLO) uplo, N, alpha, X, incX, Ap);
}

template <> inline
void syr2(const char order, const char uplo, const int N, const float alpha, const float *X, const int incX, const float *Y, const int incY, float *A, const int lda)
{
	cblas_ssyr2((CBLAS_ORDER) order, (CBLAS_UPLO) uplo, N, alpha, X, incX, Y, incY, A, lda);
}

template <> inline
void spr2(const char order, const char uplo, const int N, const float alpha, const float *X, const int incX, const float *Y, const int incY, float *A)
{
	cblas_sspr2((CBLAS_ORDER) order, (CBLAS_UPLO) uplo, N, alpha, X, incX, Y, incY, A);
}

// LEVEL 3

template <> inline
void gemm(const char order, const char transA, const char transB, const int M, const int N, const int K, const float alpha, const float *A, const int lda, const float *B, const int ldb, const float beta, float *C, const int ldc)
{
	cblas_sgemm((CBLAS_ORDER) order, (CBLAS_TRANSPOSE) transA, (CBLAS_TRANSPOSE) transB, M, N, K, alpha, A, lda, B, ldb, beta, C, ldc);
}

template <> inline
void symm(const char order, const char side, const char uplo, const int M, const int N, const float alpha, const float *A, const int lda, const float *B, const int ldb, const float beta, float *C, const int ldc)
{
	cblas_ssymm((CBLAS_ORDER) order, (CBLAS_SIDE) side, (CBLAS_UPLO) uplo, M, N, alpha, A, lda, B, ldb, beta, C, ldc);
}

template <> inline
void syrk(const char order, const char uplo, const char trans, const int N, const int K, const float alpha, const float *A, const int lda, const float beta, float *C, const int ldc)
{
	cblas_ssyrk((CBLAS_ORDER) order, (CBLAS_UPLO) uplo, (CBLAS_TRANSPOSE) trans, N, K, alpha, A, lda, beta, C, ldc);
}

template <> inline
void syr2k(const char order, const char uplo, const char trans, const int N, const int K, const float alpha, const float *A, const int lda, const float *B, const int ldb, const float beta, float *C, const int ldc)
{
	cblas_ssyr2k((CBLAS_ORDER) order, (CBLAS_UPLO) uplo, (CBLAS_TRANSPOSE) trans, N, K, alpha, A, lda, B, ldb, beta, C, ldc);
}

template <> inline
void trmm(const char order, const char side, const char uplo, const char transA, const char diag, const int M, const int N, const float alpha, const float *A, const int lda, float *B, const int ldb)
{
	cblas_strmm((CBLAS_ORDER) order, (CBLAS_SIDE) side, (CBLAS_UPLO) uplo, (CBLAS_TRANSPOSE) transA, (CBLAS_DIAG) diag, M, N, alpha, A, lda, B, ldb);
}

template <> inline
void trsm(const char order, const char side, const char uplo, const char transA, const char diag, const int M, const int N, const float alpha, const float *A, const int lda, float *B, const int ldb)
{
	cblas_strsm((CBLAS_ORDER) order, (CBLAS_SIDE) side, (CBLAS_UPLO) uplo, (CBLAS_TRANSPOSE) transA, (CBLAS_DIAG) diag, M, N, alpha, A, lda, B, ldb);
}


/////////////////////////// DOUBLE PRECISION FLOAT ////////////////////////////


// LEVEL 1

template <> inline
double dot(const int N, const double *X, const int incX, const double *Y, const int incY)
{
	return cblas_ddot(N, X, incX, Y, incY);
}

template <> inline
double nrm2(const int N, const double *X, const int incX)
{
	return cblas_dnrm2(N, X, incX);
}

template <> inline
double asum(const int N, const double *X, const int incX)
{
	return cblas_dasum(N, X, incX);
}

template <> inline
size_t iamax(const int N, const double *X, const int incX)
{
	return cblas_idamax(N, X, incX);
}

template <> inline
void swap(const int N, double *X, const int incX, double *Y, const int incY)
{
	cblas_dswap(N, X, incX, Y, incY);
}

template <> inline
void copy(const int N, const double *X, const int incX, double *Y, const int incY)
{
	cblas_dcopy(N, X, incX, Y, incY);
}

template <> inline
void axpy(const int N, const double alpha, const double *X, const int incX, double *Y, const int incY)
{
	cblas_daxpy(N, alpha, X, incX, Y, incY);
}

template <> inline
void rotg(double *a, double *b, double *c, double *s)
{
	cblas_drotg(a, b, c, s);
}

template <> inline
void rotmg(double *d1, double *d2, double *b1, const double b2, double *P)
{
	cblas_drotmg(d1, d2, b1, b2, P);
}

template <> inline
void rot(const int N, double *X, const int incX, double *Y, const int incY, const double c, const double s)
{
	cblas_drot(N, X, incX, Y, incY, c, s);
}

template <> inline
void rotm(const int N, double *X, const int incX, double *Y, const int incY, const double *P)
{
	cblas_drotm(N, X, incX, Y, incY, P);
}

template <> inline
void scal(const int N, const double alpha, double *X, const int incX)
{
	cblas_dscal(N, alpha, X, incX);
}

// LEVEL 2

template <> inline
void gemv(const char order, const char transA, const int M, const int N, const double alpha, const double *A, const int lda, const double *X, const int incX, const double beta, double *Y, const int incY)
{
	cblas_dgemv((CBLAS_ORDER) order, (CBLAS_TRANSPOSE) transA, M, N, alpha, A, lda, X, incX, beta, Y, incY);
}

template <> inline
void gbmv(const char order, const char transA, const int M, const int N, const int KL, const int KU, const double alpha, const double *A, const int lda, const double *X, const int incX, const double beta, double *Y, const int incY)
{
	cblas_dgbmv((CBLAS_ORDER) order, (CBLAS_TRANSPOSE) transA, M, N, KL, KU, alpha, A, lda, X, incX, beta, Y, incY);
}

template <> inline
void trmv(const char order, const char uplo, const char transA, const char diag, const int N, const double *A, const int lda, double *X, const int incX)
{
	cblas_dtrmv((CBLAS_ORDER) order, (CBLAS_UPLO) uplo, (CBLAS_TRANSPOSE) transA, (CBLAS_DIAG) diag, N, A, lda, X, incX);
}

template <> inline
void tbmv(const char order, const char uplo, const char transA, const char diag, const int N, const int K, const double *A, const int lda, double *X, const int incX)
{
	cblas_dtbmv((CBLAS_ORDER) order, (CBLAS_UPLO) uplo, (CBLAS_TRANSPOSE) transA, (CBLAS_DIAG) diag, N, K, A, lda, X, incX);
}

template <> inline
void tpmv(const char order, const char uplo, const char transA, const char diag, const int N, const double *Ap, double *X, const int incX)
{
	cblas_dtpmv((CBLAS_ORDER) order, (CBLAS_UPLO) uplo, (CBLAS_TRANSPOSE) transA, (CBLAS_DIAG) diag, N, Ap, X, incX);
}

template <> inline
void trsv(const char order, const char uplo, const char transA, const char diag, const int N, const double *A, const int lda, double *X, const int incX)
{
	cblas_dtrsv((CBLAS_ORDER) order, (CBLAS_UPLO) uplo, (CBLAS_TRANSPOSE) transA, (CBLAS_DIAG) diag, N, A, lda, X, incX);
}

template <> inline
void tbsv(const char order, const char uplo, const char transA, const char diag, const int N, const int K, const double *A, const int lda, double *X, const int incX)
{
	cblas_dtbsv((CBLAS_ORDER) order, (CBLAS_UPLO) uplo, (CBLAS_TRANSPOSE) transA, (CBLAS_DIAG) diag, N, K, A, lda, X, incX);
}

template <> inline
void tpsv(const char order, const char uplo, const char transA, const char diag, const int N, const double *A, double *X, const int incX)
{
	cblas_dtpsv((CBLAS_ORDER) order, (CBLAS_UPLO) uplo, (CBLAS_TRANSPOSE) transA, (CBLAS_DIAG) diag, N, A, X, incX);
}

template <> inline
void symv(const char order, const char uplo, const int N, const double alpha, const double *A, const int lda, const double *X, const int incX, const double beta, double *Y, const int incY)
{
	cblas_dsymv((CBLAS_ORDER) order, (CBLAS_UPLO) uplo, N, alpha, A, lda, X, incX, beta, Y, incY);
}

template <> inline
void sbmv(const char order, const char uplo, const int N, const int K, const double alpha, const double *A, const int lda, const double *X, const int incX, const double beta, double *Y, const int incY)
{
	cblas_dsbmv((CBLAS_ORDER) order, (CBLAS_UPLO) uplo, N, K, alpha, A, lda, X, incX, beta, Y, incY);
}

template <> inline
void spmv(const char order, const char uplo, const int N, const double alpha, const double *Ap, const double *X, const int incX, const double beta, double *Y, const int incY)
{
	cblas_dspmv((CBLAS_ORDER) order, (CBLAS_UPLO) uplo, N, alpha, Ap, X, incX, beta, Y, incY);
}

template <> inline
void ger(const char order, const int M, const int N, const double alpha, const double *X, const int incX, const double *Y, const int incY, double *A, const int lda)
{
	cblas_dger((CBLAS_ORDER) order, M, N, alpha, X, incX, Y, incY, A, lda);
}

template <> inline
void syr(const char order, const char uplo, const int N, const double alpha, const double *X, const int incX, double *A, const int lda)
{
	cblas_dsyr((CBLAS_ORDER) order, (CBLAS_UPLO) uplo, N, alpha, X, incX, A, lda);
}

template <> inline
void spr(const char order, const char uplo, const int N, const double alpha, const double *X, const int incX, double *Ap)
{
	cblas_dspr((CBLAS_ORDER) order, (CBLAS_UPLO) uplo, N, alpha, X, incX, Ap);
}

template <> inline
void syr2(const char order, const char uplo, const int N, const double alpha, const double *X, const int incX, const double *Y, const int incY, double *A, const int lda)
{
	cblas_dsyr2((CBLAS_ORDER) order, (CBLAS_UPLO) uplo, N, alpha, X, incX, Y, incY, A, lda);
}

template <> inline
void spr2(const char order, const char uplo, const int N, const double alpha, const double *X, const int incX, const double *Y, const int incY, double *A)
{
	cblas_dspr2((CBLAS_ORDER) order, (CBLAS_UPLO) uplo, N, alpha, X, incX, Y, incY, A);
}

// LEVEL 3

template <> inline
void gemm(const char order, const char transA, const char transB, const int M, const int N, const int K, const double alpha, const double *A, const int lda, const double *B, const int ldb, const double beta, double *C, const int ldc)
{
	cblas_dgemm((CBLAS_ORDER) order, (CBLAS_TRANSPOSE) transA, (CBLAS_TRANSPOSE) transB, M, N, K, alpha, A, lda, B, ldb, beta, C, ldc);
}

template <> inline
void symm(const char order, const char side, const char uplo, const int M, const int N, const double alpha, const double *A, const int lda, const double *B, const int ldb, const double beta, double *C, const int ldc)
{
	cblas_dsymm((CBLAS_ORDER) order, (CBLAS_SIDE) side, (CBLAS_UPLO) uplo, M, N, alpha, A, lda, B, ldb, beta, C, ldc);
}

template <> inline
void syrk(const char order, const char uplo, const char trans, const int N, const int K, const double alpha, const double *A, const int lda, const double beta, double *C, const int ldc)
{
	cblas_dsyrk((CBLAS_ORDER) order, (CBLAS_UPLO) uplo, (CBLAS_TRANSPOSE) trans, N, K, alpha, A, lda, beta, C, ldc);
}

template <> inline
void syr2k(const char order, const char uplo, const char trans, const int N, const int K, const double alpha, const double *A, const int lda, const double *B, const int ldb, const double beta, double *C, const int ldc)
{
	cblas_dsyr2k((CBLAS_ORDER) order, (CBLAS_UPLO) uplo, (CBLAS_TRANSPOSE) trans, N, K, alpha, A, lda, B, ldb, beta, C, ldc);
}

template <> inline
void trmm(const char order, const char side, const char uplo, const char transA, const char diag, const int M, const int N, const double alpha, const double *A, const int lda, double *B, const int ldb)
{
	cblas_dtrmm((CBLAS_ORDER) order, (CBLAS_SIDE) side, (CBLAS_UPLO) uplo, (CBLAS_TRANSPOSE) transA, (CBLAS_DIAG) diag, M, N, alpha, A, lda, B, ldb);
}

template <> inline
void trsm(const char order, const char side, const char uplo, const char transA, const char diag, const int M, const int N, const double alpha, const double *A, const int lda, double *B, const int ldb)
{
	cblas_dtrsm((CBLAS_ORDER) order, (CBLAS_SIDE) side, (CBLAS_UPLO) uplo, (CBLAS_TRANSPOSE) transA, (CBLAS_DIAG) diag, M, N, alpha, A, lda, B, ldb);
}


////////////////////////// SINGLE PRECISION COMPLEX ///////////////////////////


// LEVEL 1

template <> inline
complex<float> dotu(const int N, const complex<float> *X, const int incX, const complex<float> *Y, const int incY)
{
	complex<float> dotu;
	cblas_cdotu_sub(N, X, incX, Y, incY, &dotu);
	return dotu;
}

template <> inline
complex<float> dotc(const int N, const complex<float> *X, const int incX, const complex<float> *Y, const int incY)
{
	complex<float> dotc;
	cblas_cdotu_sub(N, X, incX, Y, incY, &dotc);
	return dotc;
}

template <> inline
float nrm2(const int N, const complex<float> *X, const int incX)
{
	return cblas_dznrm2(N, X, incX);
}

template <> inline
float asum(const int N, const complex<float> *X, const int incX)
{
	return cblas_dzasum(N, X, incX);
}

template <> inline
size_t iamax(const int N, const complex<float> *X, const int incX)
{
	return cblas_izamax(N, X, incX);
}

template <> inline
void swap(const int N, complex<float> *X, const int incX, complex<float> *Y, const int incY)
{
	cblas_cswap(N, X, incX, Y, incY);
}

template <> inline
void copy(const int N, const complex<float> *X, const int incX, complex<float> *Y, const int incY)
{
	cblas_ccopy(N, X, incX, Y, incY);
}

template <> inline
void axpy(const int N, const complex<float> alpha, const complex<float> *X, const int incX, complex<float> *Y, const int incY)
{
	cblas_caxpy(N, &alpha, X, incX, Y, incY);
}

template <> inline
void scal(const int N, const complex<float> alpha, complex<float> *X, const int incX)
{
	cblas_cscal(N, &alpha, X, incX);
}

template <> inline
void scal(const int N, const float alpha, complex<float> *X, const int incX)
{
	cblas_csscal(N, alpha, X, incX);
}

// LEVEL 2

template <> inline
void gemv(const char order, const char transA, const int M, const int N, const complex<float> alpha, const complex<float> *A, const int lda, const complex<float> *X, const int incX, const complex<float> beta, complex<float> *Y, const int incY)
{
	cblas_cgemv((CBLAS_ORDER) order, (CBLAS_TRANSPOSE) transA, M, N, &alpha, A, lda, X, incX, &beta, Y, incY);
}

template <> inline
void gbmv(const char order, const char transA, const int M, const int N, const int KL, const int KU, const complex<float> alpha, const complex<float> *A, const int lda, const complex<float> *X, const int incX, const complex<float> beta, complex<float> *Y, const int incY)
{
	cblas_cgbmv((CBLAS_ORDER) order, (CBLAS_TRANSPOSE) transA, M, N, KL, KU, &alpha, A, lda, X, incX, &beta, Y, incY);
}

template <> inline
void trmv(const char order, const char uplo, const char transA, const char diag, const int N, const complex<float> *A, const int lda, complex<float> *X, const int incX)
{
	cblas_ctrmv((CBLAS_ORDER) order, (CBLAS_UPLO) uplo, (CBLAS_TRANSPOSE) transA, (CBLAS_DIAG) diag, N, A, lda, X, incX);
}

template <> inline
void tbmv(const char order, const char uplo, const char transA, const char diag, const int N, const int K, const complex<float> *A, const int lda, complex<float> *X, const int incX)
{
	cblas_ctbmv((CBLAS_ORDER) order, (CBLAS_UPLO) uplo, (CBLAS_TRANSPOSE) transA, (CBLAS_DIAG) diag, N, K, A, lda, X, incX);
}

template <> inline
void tpmv(const char order, const char uplo, const char transA, const char diag, const int N, const complex<float> *Ap, complex<float> *X, const int incX)
{
	cblas_ctpmv((CBLAS_ORDER) order, (CBLAS_UPLO) uplo, (CBLAS_TRANSPOSE) transA, (CBLAS_DIAG) diag, N, Ap, X, incX);
}

template <> inline
void trsv(const char order, const char uplo, const char transA, const char diag, const int N, const complex<float> *A, const int lda, complex<float> *X, const int incX)
{
	cblas_ctrsv((CBLAS_ORDER) order, (CBLAS_UPLO) uplo, (CBLAS_TRANSPOSE) transA, (CBLAS_DIAG) diag, N, A, lda, X, incX);
}

template <> inline
void tbsv(const char order, const char uplo, const char transA, const char diag, const int N, const int K, const complex<float> *A, const int lda, complex<float> *X, const int incX)
{
	cblas_ctbsv((CBLAS_ORDER) order, (CBLAS_UPLO) uplo, (CBLAS_TRANSPOSE) transA, (CBLAS_DIAG) diag, N, K, A, lda, X, incX);
}

template <> inline
void tpsv(const char order, const char uplo, const char transA, const char diag, const int N, const complex<float> *A, complex<float> *X, const int incX)
{
	cblas_ctpsv((CBLAS_ORDER) order, (CBLAS_UPLO) uplo, (CBLAS_TRANSPOSE) transA, (CBLAS_DIAG) diag, N, A, X, incX);
}

template <> inline
void hemv(const char order, const char uplo, const int N, const complex<float> alpha, const complex<float> *A, const int lda, const complex<float> *X, const int incX, const complex<float> beta, complex<float> *Y, const int incY)
{
	cblas_chemv((CBLAS_ORDER) order, (CBLAS_UPLO) uplo, N, &alpha, A, lda, X, incX, &beta, Y, incY);
}

template <> inline
void hbmv(const char order, const char uplo, const int N, const int K, const complex<float> alpha, const complex<float> *A, const int lda, const complex<float> *X, const int incX, const complex<float> beta, complex<float> *Y, const int incY)
{
	cblas_chbmv((CBLAS_ORDER) order, (CBLAS_UPLO) uplo, N, K, &alpha, A, lda, X, incX, &beta, Y, incY);
}

template <> inline
void hpmv(const char order, const char uplo, const int  N, const complex<float> alpha, const complex<float> *Ap, const complex<float> *X, const int incX, const complex<float> beta, complex<float> *Y, const int incY)
{
	cblas_chpmv((CBLAS_ORDER) order, (CBLAS_UPLO) uplo, N, &alpha, Ap, X, incX, &beta, Y, incY);
}

template <> inline
void geru(const char order, const int M, const int N, const complex<float> alpha, const complex<float> *X, const int incX, const complex<float> *Y, const int incY, complex<float> *A, const int lda)
{
	cblas_cgeru((CBLAS_ORDER) order, M, N, &alpha, X, incX, Y, incY, A, lda);
}

template <> inline
void gerc(const char order, const int M, const int N, const complex<float> alpha, const complex<float> *X, const int incX, const complex<float> *Y, const int incY, complex<float> *A, const int lda)
{
	cblas_cgerc((CBLAS_ORDER) order, M, N, &alpha, X, incX, Y, incY, A, lda);
}

template <> inline
void her(const char order, const char uplo, const int N, const float alpha, const complex<float> *X, const int incX, complex<float> *A, int lda)
{
	cblas_cher((CBLAS_ORDER) order, (CBLAS_UPLO) uplo, N, alpha, X, incX, A, lda);
}

template <> inline
void hpr(const char order, const char uplo, const int N, const float alpha, const complex<float> *X, const int incX, complex<float> *A)
{
	cblas_chpr((CBLAS_ORDER) order, (CBLAS_UPLO) uplo, N, alpha, X, incX, A);
}

template <> inline
void her2(const char order, const char uplo, const int N, const complex<float> alpha, const complex<float> *X, const int incX, const complex<float> *Y, const int incY, complex<float> *A, const int lda)
{
	cblas_cher2((CBLAS_ORDER) order, (CBLAS_UPLO) uplo, N, &alpha, X, incX, Y, incY, A, lda);
}

template <> inline
void hpr2(const char order, const char uplo, const int N, const complex<float> alpha, const complex<float> *X, const int incX, const complex<float> *Y, const int incY, complex<float> *Ap)
{
	cblas_chpr2((CBLAS_ORDER) order, (CBLAS_UPLO) uplo, N, &alpha, X, incX, Y, incY, Ap);
}

// LEVEL 3

template <> inline
void gemm(const char order, const char transA, const char transB, const int M, const int N, const int K, const complex<float> alpha, const complex<float> *A, const int lda, const complex<float> *B, const int ldb, const complex<float> beta, complex<float> *C, const int ldc)
{
	cblas_cgemm((CBLAS_ORDER) order, (CBLAS_TRANSPOSE) transA, (CBLAS_TRANSPOSE) transB, M, N, K, &alpha, A, lda, B, ldb, &beta, C, ldc);
}

template <> inline
void symm(const char order, const char side, const char uplo, const int M, const int N, const complex<float> alpha, const complex<float> *A, const int lda, const complex<float> *B, const int ldb, const complex<float> beta, complex<float> *C, const int ldc)
{
	cblas_csymm((CBLAS_ORDER) order, (CBLAS_SIDE) side, (CBLAS_UPLO) uplo, M, N, &alpha, A, lda, B, ldb, &beta, C, ldc);
}

template <> inline
void syrk(const char order, const char uplo, const char trans, const int N, const int K, const complex<float> alpha, const complex<float> *A, const int lda, const complex<float> beta, complex<float> *C, const int ldc)
{
	cblas_csyrk((CBLAS_ORDER) order, (CBLAS_UPLO) uplo, (CBLAS_TRANSPOSE) trans, N, K, &alpha, A, lda, &beta, C, ldc);
}

template <> inline
void syr2k(const char order, const char uplo, const char trans, const int N, const int K, const complex<float> alpha, const complex<float> *A, const int lda, const complex<float> *B, const int ldb, const complex<float> beta, complex<float> *C, const int ldc)
{
	cblas_csyr2k((CBLAS_ORDER) order, (CBLAS_UPLO) uplo, (CBLAS_TRANSPOSE) trans, N, K, &alpha, A, lda, B, ldb, &beta, C, ldc);
}

template <> inline
void trmm(const char order, const char side, const char uplo, const char transA, const char diag, const int M, const int N, const complex<float> alpha, const complex<float> *A, const int lda, complex<float> *B, const int ldb)
{
	cblas_ctrmm((CBLAS_ORDER) order, (CBLAS_SIDE) side, (CBLAS_UPLO) uplo, (CBLAS_TRANSPOSE) transA, (CBLAS_DIAG) diag, M, N, &alpha, A, lda, B, ldb);
}

template <> inline
void trsm(const char order, const char side, const char uplo, const char transA, const char diag, const int M, const int N, const complex<float> alpha, const complex<float> *A, const int lda, complex<float> *B, const int ldb)
{
	cblas_ctrsm((CBLAS_ORDER) order, (CBLAS_SIDE) side, (CBLAS_UPLO) uplo, (CBLAS_TRANSPOSE) transA, (CBLAS_DIAG) diag, M, N, &alpha, A, lda, B, ldb);
}

template <> inline
void hemm(const char order, const char side, const char uplo, const int M, const int N, const complex<float> alpha, const complex<float> *A, const int lda, const complex<float> *B, const int ldb, const complex<float> beta, complex<float> *C, const int ldc)
{
	cblas_chemm((CBLAS_ORDER) order, (CBLAS_SIDE) side, (CBLAS_UPLO) uplo, M, N, &alpha, A, lda, B, ldb, &beta, C, ldc);
}

template <> inline
void herk(const char order, const char uplo, const char trans, const int N, const int K, const float alpha, const complex<float> *A, const int lda, const float beta, complex<float> *C, const int ldc)
{
	cblas_cherk((CBLAS_ORDER) order, (CBLAS_UPLO) uplo, (CBLAS_TRANSPOSE) trans, N, K, alpha, A, lda, beta, C, ldc);
}

template <> inline
void her2k(const char order, const char uplo, const char trans, const int N, const int K, const complex<float> alpha, const complex<float> *A, const int lda, const complex<float> *B, const int ldb, const float beta, complex<float> *C, const int ldc)
{
	cblas_cher2k((CBLAS_ORDER) order, (CBLAS_UPLO) uplo, (CBLAS_TRANSPOSE) trans, N, K, &alpha, A, lda, B, ldb, beta, C,ldc);
}


////////////////////////// DOUBLE PRECISION COMPLEX ///////////////////////////


// LEVEL 1

template <> inline
complex<double> dotu(const int N, const complex<double> *X, const int incX, const complex<double> *Y, const int incY)
{
	complex<double> dotu;
	cblas_zdotu_sub(N, X, incX, Y, incY, &dotu);
	return dotu;
}

template <> inline
complex<double> dotc(const int N, const complex<double> *X, const int incX, const complex<double> *Y, const int incY)
{
	complex<double> dotc;
	cblas_zdotu_sub(N, X, incX, Y, incY, &dotc);
	return dotc;
}

template <> inline
double nrm2(const int N, const complex<double> *X, const int incX)
{
	return cblas_dznrm2(N, X, incX);
}

template <> inline
double asum(const int N, const complex<double> *X, const int incX)
{
	return cblas_dzasum(N, X, incX);
}

template <> inline
size_t iamax(const int N, const complex<double> *X, const int incX)
{
	return cblas_izamax(N, X, incX);
}

template <> inline
void swap(const int N, complex<double> *X, const int incX, complex<double> *Y, const int incY)
{
	cblas_zswap(N, X, incX, Y, incY);
}

template <> inline
void copy(const int N, const complex<double> *X, const int incX, complex<double> *Y, const int incY)
{
	cblas_zcopy(N, X, incX, Y, incY);
}

template <> inline
void axpy(const int N, const complex<double> alpha, const complex<double> *X, const int incX, complex<double> *Y, const int incY)
{
	cblas_zaxpy(N, &alpha, X, incX, Y, incY);
}

template <> inline
void scal(const int N, const complex<double> alpha, complex<double> *X, const int incX)
{
	cblas_zscal(N, &alpha, X, incX);
}

template <> inline
void scal(const int N, const double alpha, complex<double> *X, const int incX)
{
	cblas_zdscal(N, alpha, X, incX);
}

// LEVEL 2

template <> inline
void gemv(const char order, const char transA, const int M, const int N, const complex<double> alpha, const complex<double> *A, const int lda, const complex<double> *X, const int incX, const complex<double> beta, complex<double> *Y, const int incY)
{
	cblas_zgemv((CBLAS_ORDER) order, (CBLAS_TRANSPOSE) transA, M, N, &alpha, A, lda, X, incX, &beta, Y, incY);
}

template <> inline
void gbmv(const char order, const char transA, const int M, const int N, const int KL, const int KU, const complex<double> alpha, const complex<double> *A, const int lda, const complex<double> *X, const int incX, const complex<double> beta, complex<double> *Y, const int incY)
{
	cblas_zgbmv((CBLAS_ORDER) order, (CBLAS_TRANSPOSE) transA, M, N, KL, KU, &alpha, A, lda, X, incX, &beta, Y, incY);
}

template <> inline
void trmv(const char order, const char uplo, const char transA, const char diag, const int N, const complex<double> *A, const int lda, complex<double> *X, const int incX)
{
	cblas_ztrmv((CBLAS_ORDER) order, (CBLAS_UPLO) uplo, (CBLAS_TRANSPOSE) transA, (CBLAS_DIAG) diag, N, A, lda, X, incX);
}

template <> inline
void tbmv(const char order, const char uplo, const char transA, const char diag, const int N, const int K, const complex<double> *A, const int lda, complex<double> *X, const int incX)
{
	cblas_ztbmv((CBLAS_ORDER) order, (CBLAS_UPLO) uplo, (CBLAS_TRANSPOSE) transA, (CBLAS_DIAG) diag, N, K, A, lda, X, incX);
}

template <> inline
void tpmv(const char order, const char uplo, const char transA, const char diag, const int N, const complex<double> *Ap, complex<double> *X, const int incX)
{
	cblas_ztpmv((CBLAS_ORDER) order, (CBLAS_UPLO) uplo, (CBLAS_TRANSPOSE) transA, (CBLAS_DIAG) diag, N, Ap, X, incX);
}

template <> inline
void trsv(const char order, const char uplo, const char transA, const char diag, const int N, const complex<double> *A, const int lda, complex<double> *X, const int incX)
{
	cblas_ztrsv((CBLAS_ORDER) order, (CBLAS_UPLO) uplo, (CBLAS_TRANSPOSE) transA, (CBLAS_DIAG) diag, N, A, lda, X, incX);
}

template <> inline
void tbsv(const char order, const char uplo, const char transA, const char diag, const int N, const int K, const complex<double> *A, const int lda, complex<double> *X, const int incX)
{
	cblas_ztbsv((CBLAS_ORDER) order, (CBLAS_UPLO) uplo, (CBLAS_TRANSPOSE) transA, (CBLAS_DIAG) diag, N, K, A, lda, X, incX);
}

template <> inline
void tpsv(const char order, const char uplo, const char transA, const char diag, const int N, const complex<double> *A, complex<double> *X, const int incX)
{
	cblas_ztpsv((CBLAS_ORDER) order, (CBLAS_UPLO) uplo, (CBLAS_TRANSPOSE) transA, (CBLAS_DIAG) diag, N, A, X, incX);
}

template <> inline
void hemv(const char order, const char uplo, const int N, const complex<double> alpha, const complex<double> *A, const int lda, const complex<double> *X, const int incX, const complex<double> beta, complex<double> *Y, const int incY)
{
	cblas_zhemv((CBLAS_ORDER) order, (CBLAS_UPLO) uplo, N, &alpha, A, lda, X, incX, &beta, Y, incY);
}

template <> inline
void hbmv(const char order, const char uplo, const int N, const int K, const complex<double> alpha, const complex<double> *A, const int lda, const complex<double> *X, const int incX, const complex<double> beta, complex<double> *Y, const int incY)
{
	cblas_zhbmv((CBLAS_ORDER) order, (CBLAS_UPLO) uplo, N, K, &alpha, A, lda, X, incX, &beta, Y, incY);
}

template <> inline
void hpmv(const char order, const char uplo, const int  N, const complex<double> alpha, const complex<double> *Ap, const complex<double> *X, const int incX, const complex<double> beta, complex<double> *Y, const int incY)
{
	cblas_zhpmv((CBLAS_ORDER) order, (CBLAS_UPLO) uplo, N, &alpha, Ap, X, incX, &beta, Y, incY);
}

template <> inline
void geru(const char order, const int M, const int N, const complex<double> alpha, const complex<double> *X, const int incX, const complex<double> *Y, const int incY, complex<double> *A, const int lda)
{
	cblas_zgeru((CBLAS_ORDER) order, M, N, &alpha, X, incX, Y, incY, A, lda);
}

template <> inline
void gerc(const char order, const int M, const int N, const complex<double> alpha, const complex<double> *X, const int incX, const complex<double> *Y, const int incY, complex<double> *A, const int lda)
{
	cblas_zgerc((CBLAS_ORDER) order, M, N, &alpha, X, incX, Y, incY, A, lda);
}

template <> inline
void her(const char order, const char uplo, const int N, const double alpha, const complex<double> *X, const int incX, complex<double> *A, int lda)
{
	cblas_zher((CBLAS_ORDER) order, (CBLAS_UPLO) uplo, N, alpha, X, incX, A, lda);
}

template <> inline
void hpr(const char order, const char uplo, const int N, const double alpha, const complex<double> *X, const int incX, complex<double> *A)
{
	cblas_zhpr((CBLAS_ORDER) order, (CBLAS_UPLO) uplo, N, alpha, X, incX, A);
}

template <> inline
void her2(const char order, const char uplo, const int N, const complex<double> alpha, const complex<double> *X, const int incX, const complex<double> *Y, const int incY, complex<double> *A, const int lda)
{
	cblas_zher2((CBLAS_ORDER) order, (CBLAS_UPLO) uplo, N, &alpha, X, incX, Y, incY, A, lda);
}

template <> inline
void hpr2(const char order, const char uplo, const int N, const complex<double> alpha, const complex<double> *X, const int incX, const complex<double> *Y, const int incY, complex<double> *Ap)
{
	cblas_zhpr2((CBLAS_ORDER) order, (CBLAS_UPLO) uplo, N, &alpha, X, incX, Y, incY, Ap);
}

// LEVEL 3

template <> inline
void gemm(const char order, const char transA, const char transB, const int M, const int N, const int K, const complex<double> alpha, const complex<double> *A, const int lda, const complex<double> *B, const int ldb, const complex<double> beta, complex<double> *C, const int ldc)
{
	cblas_zgemm((CBLAS_ORDER) order, (CBLAS_TRANSPOSE) transA, (CBLAS_TRANSPOSE) transB, M, N, K, &alpha, A, lda, B, ldb, &beta, C, ldc);
}

template <> inline
void symm(const char order, const char side, const char uplo, const int M, const int N, const complex<double> alpha, const complex<double> *A, const int lda, const complex<double> *B, const int ldb, const complex<double> beta, complex<double> *C, const int ldc)
{
	cblas_zsymm((CBLAS_ORDER) order, (CBLAS_SIDE) side, (CBLAS_UPLO) uplo, M, N, &alpha, A, lda, B, ldb, &beta, C, ldc);
}

template <> inline
void syrk(const char order, const char uplo, const char trans, const int N, const int K, const complex<double> alpha, const complex<double> *A, const int lda, const complex<double> beta, complex<double> *C, const int ldc)
{
	cblas_zsyrk((CBLAS_ORDER) order, (CBLAS_UPLO) uplo, (CBLAS_TRANSPOSE) trans, N, K, &alpha, A, lda, &beta, C, ldc);
}

template <> inline
void syr2k(const char order, const char uplo, const char trans, const int N, const int K, const complex<double> alpha, const complex<double> *A, const int lda, const complex<double> *B, const int ldb, const complex<double> beta, complex<double> *C, const int ldc)
{
	cblas_zsyr2k((CBLAS_ORDER) order, (CBLAS_UPLO) uplo, (CBLAS_TRANSPOSE) trans, N, K, &alpha, A, lda, B, ldb, &beta, C, ldc);
}

template <> inline
void trmm(const char order, const char side, const char uplo, const char transA, const char diag, const int M, const int N, const complex<double> alpha, const complex<double> *A, const int lda, complex<double> *B, const int ldb)
{
	cblas_ztrmm((CBLAS_ORDER) order, (CBLAS_SIDE) side, (CBLAS_UPLO) uplo, (CBLAS_TRANSPOSE) transA, (CBLAS_DIAG) diag, M, N, &alpha, A, lda, B, ldb);
}

template <> inline
void trsm(const char order, const char side, const char uplo, const char transA, const char diag, const int M, const int N, const complex<double> alpha, const complex<double> *A, const int lda, complex<double> *B, const int ldb)
{
	cblas_ztrsm((CBLAS_ORDER) order, (CBLAS_SIDE) side, (CBLAS_UPLO) uplo, (CBLAS_TRANSPOSE) transA, (CBLAS_DIAG) diag, M, N, &alpha, A, lda, B, ldb);
}

template <> inline
void hemm(const char order, const char side, const char uplo, const int M, const int N, const complex<double> alpha, const complex<double> *A, const int lda, const complex<double> *B, const int ldb, const complex<double> beta, complex<double> *C, const int ldc)
{
	cblas_zhemm((CBLAS_ORDER) order, (CBLAS_SIDE) side, (CBLAS_UPLO) uplo, M, N, &alpha, A, lda, B, ldb, &beta, C, ldc);
}

template <> inline
void herk(const char order, const char uplo, const char trans, const int N, const int K, const double alpha, const complex<double> *A, const int lda, const double beta, complex<double> *C, const int ldc)
{
	cblas_zherk((CBLAS_ORDER) order, (CBLAS_UPLO) uplo, (CBLAS_TRANSPOSE) trans, N, K, alpha, A, lda, beta, C, ldc);
}

template <> inline
void her2k(const char order, const char uplo, const char trans, const int N, const int K, const complex<double> alpha, const complex<double> *A, const int lda, const complex<double> *B, const int ldb, const double beta, complex<double> *C, const int ldc)
{
	cblas_zher2k((CBLAS_ORDER) order, (CBLAS_UPLO) uplo, (CBLAS_TRANSPOSE) trans, N, K, &alpha, A, lda, B, ldb, beta, C,ldc);
}


#endif // FILE
