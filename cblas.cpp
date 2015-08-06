#include "lux.hpp"
#include "cblas.h"

// Forwarding class to determine which numeric types get which functions

template <class number> struct cblas
{
	static int open(lua_State *state)
	{
		lux_Array<number>::open(state);
		luaL_setfuncs(state, index, 0);
		return 1;
	}

	static luaL_Reg index[];
};

// Convert between the CBLAS enumerations and strings

template <>
enum CBLAS_ORDER lux_to<enum CBLAS_ORDER>(lua_State *state, int stack)
{
	auto arg = lua_tostring(state, stack);
	switch (*arg)
	{
	case 'r':
	case 'R':
		return CblasRowMajor;
	case 'c':
	case 'C':
		return CblasColMajor;
	}
	luaL_argerror(state, stack, "R or C");
}

template <>
enum CBLAS_TRANSPOSE lux_to<enum CBLAS_TRANSPOSE>(lua_State *state, int stack)
{
	auto arg = lua_tostring(state, stack);
	switch (*arg)
	{
	case 't':
	case 'T':
		return CblasTrans;
	case ' ':
		return CblasNoTrans;
	case 'h':
	case 'H':
	case '*':
		return CblasConjTrans;
	}
	luaL_argerror(state, stack, "t, T, h, H or ' '");
}

template <>
enum CBLAS_UPLO lux_to<enum CBLAS_UPLO>(lua_State *state, int stack)
{
	auto arg = lua_tostring(state, stack);
	switch (*arg)
	{
	case 'u':
	case 'U':
		return CblasUpper;
	case 'l':
	case 'L':
		return CblasLower;
	}
	luaL_argerror(state, stack, "u, U, l, or L");
}

template <>
enum CBLAS_DIAG lux_to<enum CBLAS_DIAG>(lua_State *state, int stack)
{
	auto arg = lua_tostring(state, stack);
	switch (*arg)
	{
	case 'u':
	case 'U':
		return CblasUnit;
	case 'n':
	case 'N':
		return CblasNonUnit;
	}
	luaL_argerror(state, stack, "u, U, n, or N");
}

template <>
enum CBLAS_SIDE lux_to<enum CBLAS_SIDE>(lua_State *state, int stack)
{
	auto arg = lua_tostring(state, stack);
	switch (*arg)
	{
	case 'l':
	case 'L':
	case '<':
		return CblasLeft;
	case 'r':
	case 'R':
	case '>':
		return CblasRight;
	}
	luaL_argerror(state, stack, "l, L, <, r, R, or >");
}

// Specialization for single-precision floating-point type

template <> luaL_Reg cblas<float>::index[] =
{
	// level 1
	{"dot", lux_cast(cblas_sdot)},
	{"nrm2", lux_cast(cblas_snrm2)},
	{"asum", lux_cast(cblas_sasum)},
	{"iamax", lux_cast(cblas_isamax)},
	{"swap", lux_cast(cblas_sswap)},
	{"copy", lux_cast(cblas_scopy)},
	{"axpy", lux_cast(cblas_saxpy)},
	{"rotg", lux_cast(cblas_srotg)},
	{"rotmg", lux_cast(cblas_srotmg)},
	{"rot", lux_cast(cblas_srot)},
	{"rotm", lux_cast(cblas_srotm)},
	{"scal", lux_cast(cblas_sscal)},
	// level 2
	{"gemv", lux_cast(cblas_sgemv)},
	{"gbmv", lux_cast(cblas_sgbmv)},
	{"trmv", lux_cast(cblas_strmv)},
	{"tbmv", lux_cast(cblas_stbmv)},
	{"tpmv", lux_cast(cblas_stpmv)},
	{"trsv", lux_cast(cblas_strsv)},
	{"tbsv", lux_cast(cblas_stbsv)},
	{"tpsv", lux_cast(cblas_stpsv)},
	{"symv", lux_cast(cblas_ssymv)},
	{"sbmv", lux_cast(cblas_ssbmv)},
	{"spmv", lux_cast(cblas_sspmv)},
	{"ger", lux_cast(cblas_sger)},
	{"syr", lux_cast(cblas_ssyr)},
	{"spr", lux_cast(cblas_sspr)},
	{"syr2", lux_cast(cblas_ssyr2)},
	{"spr2", lux_cast(cblas_sspr2)},
	// level 3
	{"gemm", lux_cast(cblas_sgemm)},
	{"symm", lux_cast(cblas_ssymm)},
	{"syrk", lux_cast(cblas_ssyrk)},
	{"syr2k", lux_cast(cblas_ssyr2k)},
	{"trmm", lux_cast(cblas_strmm)},
	{"trsm", lux_cast(cblas_strsm)},
	// end
	{nullptr}
};

// Specialization for double-precision floating-point type

template <> luaL_Reg cblas<double>::index[] =
{
	// level 1
	{"dot", lux_cast(cblas_ddot)},
	{"nrm2", lux_cast(cblas_dnrm2)},
	{"asum", lux_cast(cblas_dasum)},
	{"iamax", lux_cast(cblas_idamax)},
	{"swap", lux_cast(cblas_dswap)},
	{"copy", lux_cast(cblas_dcopy)},
	{"axpy", lux_cast(cblas_daxpy)},
	{"rotg", lux_cast(cblas_drotg)},
	{"rotmg", lux_cast(cblas_drotmg)},
	{"rot", lux_cast(cblas_drot)},
	{"rotm", lux_cast(cblas_drotm)},
	{"scal", lux_cast(cblas_dscal)},
	// level 2
	{"gemv", lux_cast(cblas_dgemv)},
	{"gbmv", lux_cast(cblas_dgbmv)},
	{"trmv", lux_cast(cblas_dtrmv)},
	{"tbmv", lux_cast(cblas_dtbmv)},
	{"tpmv", lux_cast(cblas_dtpmv)},
	{"trsv", lux_cast(cblas_dtrsv)},
	{"tbsv", lux_cast(cblas_dtbsv)},
	{"tpsv", lux_cast(cblas_dtpsv)},
	{"symv", lux_cast(cblas_dsymv)},
	{"sbmv", lux_cast(cblas_dsbmv)},
	{"spmv", lux_cast(cblas_dspmv)},
	{"ger", lux_cast(cblas_dger)},
	{"syr", lux_cast(cblas_dsyr)},
	{"spr", lux_cast(cblas_dspr)},
	{"syr2", lux_cast(cblas_dsyr2)},
	{"spr2", lux_cast(cblas_dspr2)},
	// level 3
	{"gemm", lux_cast(cblas_dgemm)},
	{"symm", lux_cast(cblas_dsymm)},
	{"syrk", lux_cast(cblas_dsyrk)},
	{"syr2k", lux_cast(cblas_dsyr2k)},
	{"trmm", lux_cast(cblas_dtrmm)},
	{"trsm", lux_cast(cblas_dtrsm)},
	// end
	{nullptr}
};

// Specialization for complex single-precision floating-point type

complex<float> cblas_cdotu(const int N, const complex<float> *X, const int incX, const complex<float> *Y, const int incY)
{
	complex<float> out;
	cblas_cdotu_sub(N, X, incX, Y, incY, &out);
	return out;
}

complex<float> cblas_cdotc(const int N, const complex<float> *X, const int incX, const complex<float> *Y, const int incY)
{
	complex<float> out;
	cblas_cdotc_sub(N, X, incX, Y, incY, &out);
	return out;
}

template <> luaL_Reg cblas<complex<float>>::index[] =
{
	// level 1
	{"dot", lux_cast(cblas_cdotu)},
	{"dotc", lux_cast(cblas_cdotc)},
	{"nrm2", lux_cast(cblas_scnrm2)},
	{"asum", lux_cast(cblas_scasum)},
	{"iamax", lux_cast(cblas_icamax)},
	{"swap", lux_cast(cblas_cswap)},
	{"copy", lux_cast(cblas_ccopy)},
	{"axpy", lux_cast(cblas_caxpy)},
	{"scal", lux_cast(cblas_cscal)},
	// level 2
	{"gemv", lux_cast(cblas_cgemv)},
	{"gbmv", lux_cast(cblas_cgbmv)},
	{"trmv", lux_cast(cblas_ctrmv)},
	{"tbmv", lux_cast(cblas_ctbmv)},
	{"tpmv", lux_cast(cblas_ctpmv)},
	{"trsv", lux_cast(cblas_ctrsv)},
	{"tbsv", lux_cast(cblas_ctbsv)},
	{"tpsv", lux_cast(cblas_ctpsv)},
	{"hemv", lux_cast(cblas_chemv)},
	{"hbmv", lux_cast(cblas_chbmv)}, 
	{"hpmv", lux_cast(cblas_chpmv)},
	{"geru", lux_cast(cblas_cgeru)},
	{"gerc", lux_cast(cblas_cgerc)},
	{"her", lux_cast(cblas_cher)},
	{"hpr", lux_cast(cblas_chpr)},
	{"her2", lux_cast(cblas_cher2)},
	{"hpr2", lux_cast(cblas_chpr2)},
	// level 3
	{"gemm", lux_cast(cblas_cgemm)},
	{"symm", lux_cast(cblas_csymm)},
	{"hemm", lux_cast(cblas_chemm)},
	{"syrk", lux_cast(cblas_csyrk)},
	{"herk", lux_cast(cblas_cherk)},
	{"syr2k", lux_cast(cblas_csyr2k)},
	{"her2k", lux_cast(cblas_cher2k)},
	{"trmm", lux_cast(cblas_ctrmm)},
	{"trsm", lux_cast(cblas_ctrsm)},
	// end
	{nullptr}
};

// Specialization for complex double-precision floating-point type

complex<double> cblas_zdotu(const int N, const complex<double> *X, const int incX, const complex<double> *Y, const int incY)
{
	complex<double> out;
	cblas_zdotu_sub(N, X, incX, Y, incY, &out);
	return out;
}

complex<double> cblas_zdotc(const int N, const complex<double> *X, const int incX, const complex<double> *Y, const int incY)
{
	complex<double> out;
	cblas_zdotc_sub(N, X, incX, Y, incY, &out);
	return out;
}

template <> luaL_Reg cblas<complex<double>>::index[] =
{
	// level 1
	{"dot", lux_cast(cblas_zdotu)},
	{"dotc", lux_cast(cblas_zdotc)},
	{"nrm2", lux_cast(cblas_dznrm2)},
	{"asum", lux_cast(cblas_dzasum)},
	{"iamax", lux_cast(cblas_izamax)},
	{"swap", lux_cast(cblas_zswap)},
	{"copy", lux_cast(cblas_zcopy)},
	{"axpy", lux_cast(cblas_zaxpy)},
	{"scal", lux_cast(cblas_zscal)},
	// level 2
	{"gemv", lux_cast(cblas_zgemv)},
	{"gbmv", lux_cast(cblas_zgbmv)},
	{"trmv", lux_cast(cblas_ztrmv)},
	{"tbmv", lux_cast(cblas_ztbmv)},
	{"tpmv", lux_cast(cblas_ztpmv)},
	{"trsv", lux_cast(cblas_ztrsv)},
	{"tbsv", lux_cast(cblas_ztbsv)},
	{"tpsv", lux_cast(cblas_ztpsv)},
	{"hemv", lux_cast(cblas_zhemv)},
	{"hbmv", lux_cast(cblas_zhbmv)}, 
	{"hpmv", lux_cast(cblas_zhpmv)},
	{"geru", lux_cast(cblas_zgeru)},
	{"gerc", lux_cast(cblas_zgerc)},
	{"her", lux_cast(cblas_zher)},
	{"hpr", lux_cast(cblas_zhpr)},
	{"her2", lux_cast(cblas_zher2)},
	{"hpr2", lux_cast(cblas_zhpr2)},
	// level 3
	{"gemm", lux_cast(cblas_zgemm)},
	{"symm", lux_cast(cblas_zsymm)},
	{"hemm", lux_cast(cblas_zhemm)},
	{"syrk", lux_cast(cblas_zsyrk)},
	{"herk", lux_cast(cblas_zherk)},
	{"syr2k", lux_cast(cblas_zsyr2k)},
	{"her2k", lux_cast(cblas_zher2k)},
	{"trmm", lux_cast(cblas_ztrmm)},
	{"trsm", lux_cast(cblas_ztrsm)},
	// end
	{nullptr}
};

// Lua C module entry point

extern "C" int luaopen_cblas(lua_State *state)
{
	lua_newtable(state);
	luaL_Reg regs[] =
	{
	{"float", cblas<float>::open},
	{"double", cblas<double>::open},
	{"complex", cblas<complex<float>>::open},
	{"complexd", cblas<complex<double>>::open},
	{nullptr}
	};
	for (auto r=regs; r->name; ++r)
	{
	 luaL_requiref(state, r->name, r->func, true);
	 lua_setfield(state, -2, r->name);
	}
	return 1;
}

