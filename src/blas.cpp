/**
 * Support for CBLAS, C binding to Basic Linear Algebra Subprograms originally
 * written in Fortran 77.
 */

#include "lux.hpp"
#include "cblas.hpp"

extern "C" int luaopen_blas(lua_State *state)
{
	luaL_Reg regs[] =
	{

	// SINGLE PRECISION FLOAT

	#define S(reg) {"s" #reg, lux_cast(reg<float>)},

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

	// DOUBLE PRECISION FLOAT

	#define D(reg) {"d" #reg, lux_cast(reg<double>)},

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

	// SINGLE PRECISION COMPLEX

	#define C(reg) {"c" #reg, lux_cast(reg<complex<float>>)},

	// level 1
	C(dotu)
	C(dotc)
	{"cnrm2", lux_cast((nrm2<complex<float>, float>))},
	{"casum", lux_cast((asum<complex<float>, float>))},
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
	{"cher", lux_cast(her<float>)},
	{"chpr", lux_cast(hpr<float>)},
	C(her2)
	C(hpr2)
	// level 3
	C(gemm)
	C(symm)
	C(hemm)
	C(syrk)
	{"cherk", lux_cast(herk<float>)},
	C(syr2k)
	{"cher2k", lux_cast(her2k<float>)},
	C(trmm)
	C(trsm)

	// DOUBLE PRECISION COMPLEX

	#define Z(reg) {"z" #reg, lux_cast(reg<complex<double>>)},

	// level 1
	Z(dotu)
	Z(dotc)
	{"znrm2", lux_cast((nrm2<complex<double>, double>))},
	{"zasum", lux_cast((asum<complex<double>, double>))},
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
	{"zher", lux_cast(her<double>)},
	{"zhpr", lux_cast(hpr<double>)},
	Z(her2)
	Z(hpr2)
	// level 3
	Z(gemm)
	Z(symm)
	Z(hemm)
	Z(syrk)
	{"zherk", lux_cast(herk<double>)},
	Z(syr2k)
	{"zher2k", lux_cast(her2k<double>)},
	Z(trmm)
	Z(trsm)

	// DONE

	{nullptr}
	};

	luaL_newlib(state, regs);
	return 1;
}

