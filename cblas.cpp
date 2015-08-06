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

// Convert from strings to CBLAS enumerations

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
	// REAL VALUED

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

	// COMPLEX VALUED

	// level 1
	{"cdot", lux_cast(cblas_cdotu_sub)},
	{"cdotc", lux_cast(cblas_cdotc_sub)},
	{"cnrm2", lux_cast(cblas_scnrm2)},
	{"casum", lux_cast(cblas_scasum)},
	{"icamax", lux_cast(cblas_icamax)},
	{"cswap", lux_cast(cblas_cswap)},
	{"ccopy", lux_cast(cblas_ccopy)},
	{"caxpy", lux_cast(cblas_caxpy)},
	{"cscal", lux_cast(cblas_cscal)},
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

	{nullptr}
};

// Specialization for double-precision floating-point type

template <> luaL_Reg cblas<double>::index[] =
{
	// REAL VALUED

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

	// COMPLEX VALUED

	// level 1
	{"cdot", lux_cast(cblas_zdotu_sub)},
	{"cdotc", lux_cast(cblas_zdotc_sub)},
	{"cnrm2", lux_cast(cblas_dznrm2)},
	{"casum", lux_cast(cblas_dzasum)},
	{"icamax", lux_cast(cblas_izamax)},
	{"cswap", lux_cast(cblas_zswap)},
	{"ccopy", lux_cast(cblas_zcopy)},
	{"caxpy", lux_cast(cblas_zaxpy)},
	{"cscal", lux_cast(cblas_zscal)},
	// level 2
	{"cgemv", lux_cast(cblas_zgemv)},
	{"cgbmv", lux_cast(cblas_zgbmv)},
	{"ctrmv", lux_cast(cblas_ztrmv)},
	{"ctbmv", lux_cast(cblas_ztbmv)},
	{"ctpmv", lux_cast(cblas_ztpmv)},
	{"ctrsv", lux_cast(cblas_ztrsv)},
	{"ctbsv", lux_cast(cblas_ztbsv)},
	{"ctpsv", lux_cast(cblas_ztpsv)},
	{"chemv", lux_cast(cblas_zhemv)},
	{"chbmv", lux_cast(cblas_zhbmv)}, 
	{"chpmv", lux_cast(cblas_zhpmv)},
	{"cgeru", lux_cast(cblas_zgeru)},
	{"cgerc", lux_cast(cblas_zgerc)},
	{"cher", lux_cast(cblas_zher)},
	{"chpr", lux_cast(cblas_zhpr)},
	{"cher2", lux_cast(cblas_zher2)},
	{"chpr2", lux_cast(cblas_zhpr2)},
	// level 3
	{"cgemm", lux_cast(cblas_zgemm)},
	{"csymm", lux_cast(cblas_zsymm)},
	{"chemm", lux_cast(cblas_zhemm)},
	{"csyrk", lux_cast(cblas_zsyrk)},
	{"cherk", lux_cast(cblas_zherk)},
	{"csyr2k", lux_cast(cblas_zsyr2k)},
	{"cher2k", lux_cast(cblas_zher2k)},
	{"ctrmm", lux_cast(cblas_ztrmm)},
	{"ctrsm", lux_cast(cblas_ztrsm)},

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
	{nullptr}
	};
	for (auto r=regs; r->name; ++r)
	{
	 luaL_requiref(state, r->name, r->func, true);
	 lua_setfield(state, -2, r->name);
	}
	return 1;
}

