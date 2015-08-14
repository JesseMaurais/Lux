#include "lux.hpp"
#include "cblas.hpp"

// Rewrite these so that the we get a complex return value

static complex<float> cblas_cdotu(const int N, const complex<float> *X, const int incX, const complex<float> *Y, const int incY)
{
	complex<float> dotu;
	cblas_cdotu_sub(N, X, incX, Y, incY, &dotu);
	return dotu;
}

static complex<float> cblas_cdotc(const int N, const complex<float> *X, const int incX, const complex<float> *Y, const int incY)
{
	complex<float> dotc;
	cblas_cdotu_sub(N, X, incX, Y, incY, &dotc);
	return dotc;
}

static complex<double> cblas_zdotu(const int N, const complex<double> *X, const int incX, const complex<double> *Y, const int incY)
{
	complex<double> dotu;
	cblas_zdotu_sub(N, X, incX, Y, incY, &dotu);
	return dotu;
}

static complex<double> cblas_zdotc(const int N, const complex<double> *X, const int incX, const complex<double> *Y, const int incY)
{
	complex<double> dotc;
	cblas_zdotu_sub(N, X, incX, Y, incY, &dotc);
	return dotc;
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
	{"cdotu", lux_cast(cblas_cdotu)},
	{"cdotc", lux_cast(cblas_cdotc)},
	{"cnrm2", lux_cast(cblas_scnrm2)},
	{"casum", lux_cast(cblas_scasum)},
	{"icamax", lux_cast(cblas_icamax)},
	{"cswap", lux_cast(cblas_cswap)},
	{"ccopy", lux_cast(cblas_ccopy)},
	{"caxpy", lux_cast(cblas_caxpy)},
	{"cscal", lux_cast(cblas_cscal)},
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

	// SINGLE PRECISION REAL

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
	{"zdotu", lux_cast(cblas_zdotu)},
	{"zdotc", lux_cast(cblas_zdotc)},
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

