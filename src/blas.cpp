/**
 * Support for CBLAS, C binding to Basic Linear Algebra Subprograms originally
 * written in Fortran 77.
 */

#include "lux.hpp"
#include "cblas.hpp"

template <class number> struct CBLAS
{
	typedef lux_Store<number*> Type;

	static int open(lua_State *state)
	{
		if (luaL_getmetatable(state, Type::name))
		{
			luaL_setfuncs(state, regs, 0);
			lua_pop(state, 1);
			return 0;
		}
		return luaL_error(state, "must require arrays first");
	}

	static luaL_Reg regs[];
};

// SINGLE PRECISION FLOAT

#define S(reg) {#reg, lux_cast(reg<float>)},

template <> luaL_Reg CBLAS<float>::regs[] =
{
	// level 1
	S(dot)
	S(nrm2)
	S(asum)
	S(iamax)
	S(swap)
	S(copy)
	S(axpy)
	S(rotg)
	S(rotmg)
	S(rot)
	S(rotm)
	S(scal)
	// level 2
	S(gemv)
	S(gbmv)
	S(trmv)
	S(tbmv)
	S(tpmv)
	S(trsv)
	S(tbsv)
	S(tpsv)
	S(symv)
	S(sbmv)
	S(spmv)
	S(ger)
	S(syr)
	S(spr)
	S(syr2)
	S(spr2)
	// level 2
	S(gemm)
	S(symm)
	S(syrk)
	S(syr2k)
	S(trmm)
	S(trsm)
	// end
	{nullptr}
};

// DOUBLE PRECISION FLOAT

#define D(reg) {#reg, lux_cast(reg<double>)},

template <> luaL_Reg CBLAS<double>::regs[] =
{
	// level 1
	D(dot)
	D(nrm2)
	D(asum)
	D(iamax)
	D(swap)
	D(copy)
	D(axpy)
	D(rotg)
	D(rotmg)
	D(rot)
	D(rotm)
	D(scal)
	// level 2
	D(gemv)
	D(gbmv)
	D(trmv)
	D(tbmv)
	D(tpmv)
	D(trsv)
	D(tbsv)
	D(tpsv)
	D(symv)
	D(sbmv)
	D(spmv)
	D(ger)
	D(syr)
	D(spr)
	D(syr2)
	D(spr2)
	// level 2
	D(gemm)
	D(symm)
	D(syrk)
	D(syr2k)
	D(trmm)
	D(trsm)
	// end
	{nullptr}
};

// SINGLE PRECISION COMPLEX

#define C(reg) {#reg, lux_cast(reg<complex<float>>)},

template <> luaL_Reg CBLAS<complex<float>>::regs[] =
{
	// level 1
	C(dotu)
	C(dotc)
	{"nrm2", lux_cast((nrm2<complex<float>, float>))},
	{"asum", lux_cast((asum<complex<float>, float>))},
	C(iamax)
	C(swap)
	C(copy)
	C(axpy)
	C(scal)
	// level 2
	C(gemv)
	C(gbmv)
	C(trmv)
	C(tbmv)
	C(tpmv)
	C(trsv)
	C(tbsv)
	C(tpsv)
	C(hemv)
	C(hbmv)
	C(hpmv)
	C(geru)
	C(gerc)
	{"her", lux_cast(her<float>)},
	{"hpr", lux_cast(hpr<float>)},
	C(her2)
	C(hpr2)
	// level 3
	C(gemm)
	C(symm)
	C(hemm)
	C(syrk)
	{"herk", lux_cast(herk<float>)},
	C(syr2k)
	{"her2k", lux_cast(her2k<float>)},
	C(trmm)
	C(trsm)
	// end
	{nullptr}
};

// DOUBLE PRECISION COMPLEX

#define Z(reg) {#reg, lux_cast(reg<complex<double>>)},

template <> luaL_Reg CBLAS<complex<double>>::regs[] =
{
	// level 1
	Z(dotu)
	Z(dotc)
	{"nrm2", lux_cast((nrm2<complex<double>, double>))},
	{"asum", lux_cast((asum<complex<double>, double>))},
	Z(iamax)
	Z(swap)
	Z(copy)
	Z(axpy)
	Z(scal)
	// level 2
	Z(gemv)
	Z(gbmv)
	Z(trmv)
	Z(tbmv)
	Z(tpmv)
	Z(trsv)
	Z(tbsv)
	Z(tpsv)
	Z(hemv)
	Z(hbmv)
	Z(hpmv)
	Z(geru)
	Z(gerc)
	{"her", lux_cast(her<double>)},
	{"hpr", lux_cast(hpr<double>)},
	Z(her2)
	Z(hpr2)
	// level 3
	Z(gemm)
	Z(symm)
	Z(hemm)
	Z(syrk)
	{"herk", lux_cast(herk<double>)},
	Z(syr2k)
	{"her2k", lux_cast(her2k<double>)},
	Z(trmm)
	Z(trsm)
	// end
	{nullptr}
};

// Lua module entry point

extern "C" int luaopen_blas(lua_State *state)
{
	CBLAS<float>::open(state);	
	CBLAS<double>::open(state);
	CBLAS<complex<float>>::open(state);
	CBLAS<complex<double>>::open(state);
	return 0;
}

