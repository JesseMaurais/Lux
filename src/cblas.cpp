/**
 * Support for CBLAS the C binding of Basic Linear Algebra Subprograms.
 */

#include "lux.hpp"
#include "cblas.hpp"

// Rewrite these so that the we get a complex return value

static inline
 complex<float> cdotu(const int N, const complex<float> *X, const int incX, const complex<float> *Y, const int incY)
{
	complex<float> dotu;
	cblas_cdotu_sub(N, X, incX, Y, incY, &dotu);
	return dotu;
}

static inline
complex<float> cdotc(const int N, const complex<float> *X, const int incX, const complex<float> *Y, const int incY)
{
	complex<float> dotc;
	cblas_cdotu_sub(N, X, incX, Y, incY, &dotc);
	return dotc;
}

static inline
complex<double> zdotu(const int N, const complex<double> *X, const int incX, const complex<double> *Y, const int incY)
{
	complex<double> dotu;
	cblas_zdotu_sub(N, X, incX, Y, incY, &dotu);
	return dotu;
}

static inline
complex<double> zdotc(const int N, const complex<double> *X, const int incX, const complex<double> *Y, const int incY)
{
	complex<double> dotc;
	cblas_zdotu_sub(N, X, incX, Y, incY, &dotc);
	return dotc;
}

static inline
void cscal(const int N, const complex<float> alpha, complex<float> *X, const int incX)
{
	cblas_cscal(N, &alpha, X, incX);
}

static inline
void zscal(const int N, const complex<double> alpha, complex<double> *X, const int incX)
{
	cblas_zscal(N, &alpha, X, incX);
}

static inline
void cgemv(const char order, const char transA, const int M, const int N, const complex<float> alpha, const complex<float> *A, const int lda, const complex<float> *X, const int incX, const complex<float> beta, complex<float> *Y, const int incY)
{
	cblas_cgemv(order, transA, M, N, &alpha, A, lda, X, incX, &beta, Y, incY);
}

static inline
void zgemv(const char order, const char transA, const int M, const int N, const complex<double> alpha, const complex<double> *A, const int lda, const complex<double> *X, const int incX, const complex<double> beta, complex<double> *Y, const int incY)
{
	cblas_zgemv(order, transA, M, N, &alpha, A, lda, X, incX, &beta, Y, incY);
}

static inline
void cgbmv(const char order, const char transA, const int M, const int N, const int KL, const int KU, const complex<float> alpha, const complex<float> *A, const int lda, const complex<float> *X, const int incX, const complex<float> beta, complex<float> *Y, const int incY)
{
	cblas_cgbmv(order, transA, M, N, KL, KU, &alpha, A, lda, X, incX, &beta, Y, incY);
}

static inline
void zgbmv(const char order, const char transA, const int M, const int N, const int KL, const int KU, const complex<double> alpha, const complex<double> *A, const int lda, const complex<double> *X, const int incX, const complex<double> beta, complex<double> *Y, const int incY)
{
	cblas_zgbmv(order, transA, M, N, KL, KU, &alpha, A, lda, X, incX, &beta, Y, incY);
}

static inline
void chemv(const char order, const char uplo, const int N, const complex<float> alpha, const complex<float> *A, const int lda, const complex<float> *X, const int incX, const complex<float> beta, complex<float> *Y, const int incY)
{
	cblas_chemv(order, uplo, N, &alpha, A, lda, X, incX, &beta, Y, incY);
}

static inline
void zhemv(const char order, const char uplo, const int N, const complex<double> alpha, const complex<double> *A, const int lda, const complex<double> *X, const int incX, const complex<double> beta, complex<double> *Y, const int incY)
{
	cblas_zhemv(order, uplo, N, &alpha, A, lda, X, incX, &beta, Y, incY);
}

static inline
void chbmv(const char order, const char uplo, const int N, const int K, const complex<float> alpha, const complex<float> *A, const int lda, const complex<float> *X, const int incX, const complex<float> beta, complex<float> *Y, const int incY)
{
	cblas_chbmv(order, uplo, N, K, &alpha, A, lda, X, incX, &beta, Y, incY);
}

static inline
void zhbmv(const char order, const char uplo, const int N, const int K, const complex<double> alpha, const complex<double> *A, const int lda, const complex<double> *X, const int incX, const complex<double> beta, complex<double> *Y, const int incY)
{
	cblas_zhbmv(order, uplo, N, K, &alpha, A, lda, X, incX, &beta, Y, incY);
}

static inline
void chpmv(const char order, const char uplo, const int  N, const complex<float> alpha, const complex<float> *Ap, const complex<float> *X, const int incX, const complex<float> beta, complex<float> *Y, const int incY)
{
	cblas_chpmv(order, uplo, N, &alpha, Ap, X, incX, &beta, Y, incY);
}

static inline
void zhpmv(const char order, const char uplo, const int  N, const complex<double> alpha, const complex<double> *Ap, const complex<double> *X, const int incX, const complex<double> beta, complex<double> *Y, const int incY)
{
	cblas_zhpmv(order, uplo, N, &alpha, Ap, X, incX, &beta, Y, incY);
}

static inline
void cgeru(const char order, const int M, const int N, const complex<float> alpha, const complex<float> *X, const int incX, const complex<float> *Y, const int incY, complex<float> *A, const int lda)
{
	cblas_cgeru(order, M, N, &alpha, X, incX, Y, incY, A, lda);
}

static inline
void zgeru(const char order, const int M, const int N, const complex<double> alpha, const complex<double> *X, const int incX, const complex<double> *Y, const int incY, complex<double> *A, const int lda)
{
	cblas_zgeru(order, M, N, &alpha, X, incX, Y, incY, A, lda);
}

static inline
void cgerc(const char order, const int M, const int N, const complex<float> alpha, const complex<float> *X, const int incX, const complex<float> *Y, const int incY, complex<float> *A, const int lda)
{
	cblas_cgerc(order, M, N, &alpha, X, incX, Y, incY, A, lda);
}

static inline
void zgerc(const char order, const int M, const int N, const complex<double> alpha, const complex<double> *X, const int incX, const complex<double> *Y, const int incY, complex<double> *A, const int lda)
{
	cblas_zgerc(order, M, N, &alpha, X, incX, Y, incY, A, lda);
}

static inline
void cher2(const char order, const char uplo, const int N, const complex<float> alpha, const complex<float> *X, const int incX, const complex<float> *Y, const int incY, complex<float> *A, const int lda)
{
	cblas_cher2(order, uplo, N, &alpha, X, incX, Y, incY, A, lda);
}

static inline
void zher2(const char order, const char uplo, const int N, const complex<double> alpha, const complex<double> *X, const int incX, const complex<double> *Y, const int incY, complex<double> *A, const int lda)
{
	cblas_zher2(order, uplo, N, &alpha, X, incX, Y, incY, A, lda);
}

static inline
void chpr2(const char order, const char uplo, const int N, const complex<float> alpha, const complex<float> *X, const int incX, const complex<float> *Y, const int incY, complex<float> *Ap)
{
	cblas_chpr2(order, uplo, N, &alpha, X, incX, Y, incY, Ap);
}

static inline
void zhpr2(const char order, const char uplo, const int N, const complex<double> alpha, const complex<double> *X, const int incX, const complex<double> *Y, const int incY, complex<double> *Ap)
{
	cblas_zhpr2(order, uplo, N, &alpha, X, incX, Y, incY, Ap);
}

static inline
void cgemm(const char order, const char transA, const char transB, const int M, const int N, const int K, const complex<float> alpha, const complex<float> *A, const int lda, const complex<float> *B, const int ldb, const complex<float> beta, complex<float> *C, const int ldc)
{
	cblas_cgemm(order, transA, transB, M, N, K, &alpha, A, lda, B, ldb, &beta, C, ldc);
}

static inline
void zgemm(const char order, const char transA, const char transB, const int M, const int N, const int K, const complex<double> alpha, const complex<double> *A, const int lda, const complex<double> *B, const int ldb, const complex<double> beta, complex<double> *C, const int ldc)
{
	cblas_zgemm(order, transA, transB, M, N, K, &alpha, A, lda, B, ldb, &beta, C, ldc);
}

static inline
void csymm(const char order, const char side, const char uplo, const int M, const int N, const complex<float> alpha, const complex<float> *A, const int lda, const complex<float> *B, const int ldb, const complex<float> beta, complex<float> *C, const int ldc)
{
	cblas_csymm(order, side, uplo, M, N, &alpha, A, lda, B, ldb, &beta, C, ldc);
}

static inline
void zsymm(const char order, const char side, const char uplo, const int M, const int N, const complex<double> alpha, const complex<double> *A, const int lda, const complex<double> *B, const int ldb, const complex<double> beta, complex<double> *C, const int ldc)
{
	cblas_zsymm(order, side, uplo, M, N, &alpha, A, lda, B, ldb, &beta, C, ldc);
}

static inline
void csyrk(const char order, const char uplo, const char trans, const int N, const int K, const complex<float> alpha, const complex<float> *A, const int lda, const complex<float> beta, complex<float> *C, const int ldc)
{
	cblas_csyrk(order, uplo, trans, N, K, &alpha, A, lda, &beta, C, ldc);
}

static inline
void zsyrk(const char order, const char uplo, const char trans, const int N, const int K, const complex<double> alpha, const complex<double> *A, const int lda, const complex<double> beta, complex<double> *C, const int ldc)
{
	cblas_zsyrk(order, uplo, trans, N, K, &alpha, A, lda, &beta, C, ldc);
}

static inline
void csyr2k(const char order, const char uplo, const char trans, const int N, const int K, const complex<float> alpha, const complex<float> *A, const int lda, const complex<float> *B, const int ldb, const complex<float> beta, complex<float> *C, const int ldc)
{
	cblas_csyr2k(order, uplo, trans, N, K, &alpha, A, lda, B, ldb, &beta, C, ldc);
}

static inline
void zsyr2k(const char order, const char uplo, const char trans, const int N, const int K, const complex<double> alpha, const complex<double> *A, const int lda, const complex<double> *B, const int ldb, const complex<double> beta, complex<double> *C, const int ldc)
{
	cblas_zsyr2k(order, uplo, trans, N, K, &alpha, A, lda, B, ldb, &beta, C, ldc);
}

static inline
void ctrmm(const char order, const char side, const char uplo, const char transA, const char diag, const int M, const int N, const complex<float> alpha, const complex<float> *A, const int lda, complex<float> *B, const int ldb)
{
	cblas_ctrmm(order, side, uplo, transA, diag, M, N, &alpha, A, lda, B, ldb);
}

static inline
void ztrmm(const char order, const char side, const char uplo, const char transA, const char diag, const int M, const int N, const complex<double> alpha, const complex<double> *A, const int lda, complex<double> *B, const int ldb)
{
	cblas_ztrmm(order, side, uplo, transA, diag, M, N, &alpha, A, lda, B, ldb);
}

static inline
void ctrsm(const char order, const char side, const char uplo, const char transA, const char diag, const int M, const int N, const complex<float> alpha, const complex<float> *A, const int lda, complex<float> *B, const int ldb)
{
	cblas_ctrsm(order, side, uplo, transA, diag, M, N, &alpha, A, lda, B, ldb);
}

static inline
void ztrsm(const char order, const char side, const char uplo, const char transA, const char diag, const int M, const int N, const complex<double> alpha, const complex<double> *A, const int lda, complex<double> *B, const int ldb)
{
	cblas_ztrsm(order, side, uplo, transA, diag, M, N, &alpha, A, lda, B, ldb);
}

static inline
void chemm(const char order, const char side, const char uplo, const int M, const int N, const complex<float> alpha, const complex<float> *A, const int lda, const complex<float> *B, const int ldb, const complex<float> beta, complex<float> *C, const int ldc)
{
	cblas_chemm(order, side, uplo, M, N, &alpha, A, lda, B, ldb, &beta, C, ldc);
}

static inline
void zhemm(const char order, const char side, const char uplo, const int M, const int N, const complex<double> alpha, const complex<double> *A, const int lda, const complex<double> *B, const int ldb, const complex<double> beta, complex<double> *C, const int ldc)
{
	cblas_zhemm(order, side, uplo, M, N, &alpha, A, lda, B, ldb, &beta, C, ldc);
}

static inline
void cherk(const char order, const char uplo, const char trans, const int N, const int K, const float alpha, const complex<float> *A, const int lda, const float beta, complex<float> *C, const int ldc)
{
	cblas_cherk(order, uplo, trans, N, K, alpha, A, lda, beta, C, ldc);
}

static inline
void zherk(const char order, const char uplo, const char trans, const int N, const int K, const double alpha, const complex<double> *A, const int lda, const double beta, complex<double> *C, const int ldc)
{
	cblas_zherk(order, uplo, trans, N, K, alpha, A, lda, beta, C, ldc);
}

static inline
void cher2k(const char order, const char uplo, const char trans, const int N, const int K, const complex<float> alpha, const complex<float> *A, const int lda, const complex<float> *B, const int ldb, const float beta, complex<float> *C, const int ldc)
{
	cblas_cher2k(order, uplo, trans, N, K, &alpha, A, lda, B, ldb, beta, C,ldc);
}

static inline
void zher2k(const char order, const char uplo, const char trans, const int N, const int K, const complex<double> alpha, const complex<double> *A, const int lda, const complex<double> *B, const int ldb, const double beta, complex<double> *C, const int ldc)
{
	cblas_zher2k(order, uplo, trans, N, K, &alpha, A, lda, B, ldb, beta, C,ldc);
}


// Lua C module entry point

extern "C" int luaopen_cblas(lua_State *state)
{
	luaL_Reg regs[] = 
	{
	// SINGLE PRECISION REAL

	// level 1
	{"sdsdot", lux_cast(cblas_sdsdot)},
	{"dsdot", lux_cast(cblas_dsdot)},
	{"sdot", lux_cast(cblas_sdot)},
	{"snrm2", lux_cast(cblas_snrm2)},
	{"sasum", lux_cast(cblas_sasum)},
	{"siamax", lux_cast(cblas_isamax)},
	{"sswap", lux_cast(cblas_sswap)},
	{"scopy", lux_cast(cblas_scopy)},
	{"saxpy", lux_cast(cblas_saxpy)},
	{"srotg", lux_cast(cblas_srotg)},
	{"srotmg", lux_cast(cblas_srotmg)},
	{"srot", lux_cast(cblas_srot)},
	{"srotm", lux_cast(cblas_srotm)},
	{"sscal", lux_cast(cblas_sscal)},
	// level 2
	{"sgemv", lux_cast(cblas_sgemv)},
	{"sgbmv", lux_cast(cblas_sgbmv)},
	{"strmv", lux_cast(cblas_strmv)},
	{"stbmv", lux_cast(cblas_stbmv)},
	{"stpmv", lux_cast(cblas_stpmv)},
	{"strsv", lux_cast(cblas_strsv)},
	{"stbsv", lux_cast(cblas_stbsv)},
	{"stpsv", lux_cast(cblas_stpsv)},
	{"ssymv", lux_cast(cblas_ssymv)},
	{"ssbmv", lux_cast(cblas_ssbmv)},
	{"sspmv", lux_cast(cblas_sspmv)},
	{"sger", lux_cast(cblas_sger)},
	{"ssyr", lux_cast(cblas_ssyr)},
	{"sspr", lux_cast(cblas_sspr)},
	{"ssyr2", lux_cast(cblas_ssyr2)},
	{"sspr2", lux_cast(cblas_sspr2)},
	// level 3
	{"sgemm", lux_cast(cblas_sgemm)},
	{"ssymm", lux_cast(cblas_ssymm)},
	{"ssyrk", lux_cast(cblas_ssyrk)},
	{"ssyr2k", lux_cast(cblas_ssyr2k)},
	{"strmm", lux_cast(cblas_strmm)},
	{"strsm", lux_cast(cblas_strsm)},

	// SINGLE PRECISION COMPLEX

	// level 1
	{"cdotu", lux_cast(cdotu)},
	{"cdotc", lux_cast(cdotc)},
	{"cnrm2", lux_cast(cblas_scnrm2)},
	{"casum", lux_cast(cblas_scasum)},
	{"icamax", lux_cast(cblas_icamax)},
	{"cswap", lux_cast(cblas_cswap)},
	{"ccopy", lux_cast(cblas_ccopy)},
	{"caxpy", lux_cast(cblas_caxpy)},
	{"cscal", lux_cast(cscal)},
	{"csscal", lux_cast(cblas_csscal)},
	// level 2
	{"cgemv", lux_cast(cblas_cgemv)},
	{"cgbmv", lux_cast(cblas_cgbmv)},
	{"ctrmv", lux_cast(cblas_ctrmv)},
	{"ctbmv", lux_cast(cblas_ctbmv)},
	{"ctpmv", lux_cast(cblas_ctpmv)},
	{"ctrsv", lux_cast(cblas_ctrsv)},
	{"ctbsv", lux_cast(cblas_ctbsv)},
	{"ctpsv", lux_cast(cblas_ctpsv)},
	{"chemv", lux_cast(cblas_chemv)},
	{"chbmv", lux_cast(cblas_chbmv)}, 
	{"chpmv", lux_cast(cblas_chpmv)},
	{"cgeru", lux_cast(cblas_cgeru)},
	{"cgerc", lux_cast(cblas_cgerc)},
	{"cher", lux_cast(cblas_cher)},
	{"chpr", lux_cast(cblas_chpr)},
	{"cher2", lux_cast(cblas_cher2)},
	{"chpr2", lux_cast(cblas_chpr2)},
	// level 3
	{"cgemm", lux_cast(cblas_cgemm)},
	{"csymm", lux_cast(cblas_csymm)},
	{"chemm", lux_cast(cblas_chemm)},
	{"csyrk", lux_cast(cblas_csyrk)},
	{"cherk", lux_cast(cblas_cherk)},
	{"csyr2k", lux_cast(cblas_csyr2k)},
	{"cher2k", lux_cast(cblas_cher2k)},
	{"ctrmm", lux_cast(cblas_ctrmm)},
	{"ctrsm", lux_cast(cblas_ctrsm)},

	// DOUBLE PRECISION REAL

	// level 1
	{"ddot", lux_cast(cblas_ddot)},
	{"dnrm2", lux_cast(cblas_dnrm2)},
	{"dasum", lux_cast(cblas_dasum)},
	{"idamax", lux_cast(cblas_idamax)},
	{"dswap", lux_cast(cblas_dswap)},
	{"dcopy", lux_cast(cblas_dcopy)},
	{"daxpy", lux_cast(cblas_daxpy)},
	{"drotg", lux_cast(cblas_drotg)},
	{"drotmg", lux_cast(cblas_drotmg)},
	{"drot", lux_cast(cblas_drot)},
	{"drotm", lux_cast(cblas_drotm)},
	{"dscal", lux_cast(cblas_dscal)},
	// level 2
	{"dgemv", lux_cast(cblas_dgemv)},
	{"dgbmv", lux_cast(cblas_dgbmv)},
	{"dtrmv", lux_cast(cblas_dtrmv)},
	{"dtbmv", lux_cast(cblas_dtbmv)},
	{"dtpmv", lux_cast(cblas_dtpmv)},
	{"dtrsv", lux_cast(cblas_dtrsv)},
	{"dtbsv", lux_cast(cblas_dtbsv)},
	{"dtpsv", lux_cast(cblas_dtpsv)},
	{"dsymv", lux_cast(cblas_dsymv)},
	{"dsbmv", lux_cast(cblas_dsbmv)},
	{"dspmv", lux_cast(cblas_dspmv)},
	{"dger", lux_cast(cblas_dger)},
	{"dsyr", lux_cast(cblas_dsyr)},
	{"dspr", lux_cast(cblas_dspr)},
	{"dsyr2", lux_cast(cblas_dsyr2)},
	{"dspr2", lux_cast(cblas_dspr2)},
	// level 3
	{"dgemm", lux_cast(cblas_dgemm)},
	{"dsymm", lux_cast(cblas_dsymm)},
	{"dsyrk", lux_cast(cblas_dsyrk)},
	{"dsyr2k", lux_cast(cblas_dsyr2k)},
	{"dtrmm", lux_cast(cblas_dtrmm)},
	{"dtrsm", lux_cast(cblas_dtrsm)},

	// DOUBLE PRECISION COMPLEX

	// level 1
	{"zdotu", lux_cast(zdotu)},
	{"zdotc", lux_cast(zdotc)},
	{"znrm2", lux_cast(cblas_dznrm2)},
	{"zasum", lux_cast(cblas_dzasum)},
	{"izamax", lux_cast(cblas_izamax)},
	{"zswap", lux_cast(cblas_zswap)},
	{"zcopy", lux_cast(cblas_zcopy)},
	{"zaxpy", lux_cast(cblas_zaxpy)},
	{"zscal", lux_cast(cblas_zscal)},
	{"zdscal", lux_cast(cblas_zdscal)},
	// level 2
	{"zgemv", lux_cast(cblas_zgemv)},
	{"zgbmv", lux_cast(cblas_zgbmv)},
	{"ztrmv", lux_cast(cblas_ztrmv)},
	{"ztbmv", lux_cast(cblas_ztbmv)},
	{"ztpmv", lux_cast(cblas_ztpmv)},
	{"ztrsv", lux_cast(cblas_ztrsv)},
	{"ztbsv", lux_cast(cblas_ztbsv)},
	{"ztpsv", lux_cast(cblas_ztpsv)},
	{"zhemv", lux_cast(cblas_zhemv)},
	{"zhbmv", lux_cast(cblas_zhbmv)}, 
	{"zhpmv", lux_cast(cblas_zhpmv)},
	{"zgeru", lux_cast(cblas_zgeru)},
	{"zgerc", lux_cast(cblas_zgerc)},
	{"zher", lux_cast(cblas_zher)},
	{"zhpr", lux_cast(cblas_zhpr)},
	{"zher2", lux_cast(cblas_zher2)},
	{"zhpr2", lux_cast(cblas_zhpr2)},
	// level 3
	{"zgemm", lux_cast(cblas_zgemm)},
	{"zsymm", lux_cast(cblas_zsymm)},
	{"zhemm", lux_cast(cblas_zhemm)},
	{"zsyrk", lux_cast(cblas_zsyrk)},
	{"zherk", lux_cast(cblas_zherk)},
	{"zsyr2k", lux_cast(cblas_zsyr2k)},
	{"zher2k", lux_cast(cblas_zher2k)},
	{"ztrmm", lux_cast(cblas_ztrmm)},
	{"ztrsm", lux_cast(cblas_ztrsm)},

	{nullptr}
	};
	luaL_newlib(state, regs);
	return 1;
}

