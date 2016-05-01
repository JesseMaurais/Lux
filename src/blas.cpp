/**
 * Support for CBLAS, C binding to Basic Linear Algebra Subprograms originally
 * written in Fortran 77. Linking to Intel's MKL should be transparent through
 * C into Lua. NVidia's CUDA implementation is stateful, however, and requires
 * extra wrapping.
 */

#include "lux.hpp"
#include "cblas.hpp"


extern "C" int luaopen_blas(lua_State *state)
{
	// SINGLE PRECISION FLOAT

	#define s(reg) {#reg, lux_cast(reg<float>)},

	luaL_Reg sreg [] =
	{
		// LEVEL 1
		s(dot)
		s(nrm2)
		s(asum)
		s(iamax)
		s(swap)
		s(copy)
		s(axpy)
		s(rotg)
		s(rotmg)
		s(rot)
		s(rotm)
		s(scal)
		// LEVEL 2
		s(gemv)
		s(gbmv)
		s(trmv)
		s(tbmv)
		s(tpmv)
		s(trsv)
		s(tbsv)
		s(tpsv)
		s(symv)
		s(sbmv)
		s(spmv)
		s(ger)
		s(syr)
		s(spr)
		s(syr2)
		s(spr2)
		// LEVEL 3
		s(gemm)
		s(symm)
		s(syrk)
		s(syr2k)
		s(trmm)
		s(trsm)
		// END
		{nullptr}
	};
	
	// DOUBLE PRECISION FLOAT

	#define d(reg) {#reg, lux_cast(reg<double>)},

	luaL_Reg dreg [] =
	{
		// LEVEL 1
		d(dot)
		d(nrm2)
		d(asum)
		d(iamax)
		d(swap)
		d(copy)
		d(axpy)
		d(rotg)
		d(rotmg)
		d(rot)
		d(rotm)
		d(scal)
		// LEVEL 2
		d(gemv)
		d(gbmv)
		d(trmv)
		d(tbmv)
		d(tpmv)
		d(trsv)
		d(tbsv)
		d(tpsv)
		d(symv)
		d(sbmv)
		d(spmv)
		d(ger)
		d(syr)
		d(spr)
		d(syr2)
		d(spr2)
		// LEVEL 3
		d(gemm)
		d(symm)
		d(syrk)
		d(syr2k)
		d(trmm)
		d(trsm)
		// END
		{nullptr}
	};
	
	// SINGLE PRECISION COMPLEX

	#define c(reg) {#reg, lux_cast(reg<complex<float>>)},

	luaL_Reg creg [] =
	{
		// LEVEL 1
		c(dotu)
		c(dotc)
//		c(nrm2)
		{"nrm2", lux_cast((nrm2<complex<float>, float>))},
//		c(asum)
		{"asum", lux_cast((asum<complex<float>, float>))},
		c(iamax)
		c(swap)
		c(copy)
		c(axpy)
		c(scal)
		// LEVEL 2
		c(gemv)
		c(gbmv)
		c(trmv)
		c(tbmv)
		c(tpmv)
		c(trsv)
		c(tbsv)
		c(tpsv)
		c(hemv)
		c(hbmv)
		c(hpmv)
		c(geru)
		c(gerc)
//		c(her)
		{"her", lux_cast(her<float>)},
//		c(hpr)
		{"hpr", lux_cast(hpr<float>)},
		c(her2)
		c(hpr2)
		// LEVEL 3
		c(gemm)
		c(symm)
		c(hemm)
		c(syrk)
//		c(herk)
		{"herk", lux_cast(herk<float>)},
		c(syr2k)
//		c(her2k)
		{"her2k", lux_cast(her2k<float>)},
		c(trmm)
		c(trsm)
		// END
		{nullptr}
	};
	
	// DOUBLE PRECISION COMPLEX

	#define z(reg) {#reg, lux_cast(reg<complex<double>>)},

	luaL_Reg zreg [] =
	{
		// LEVEL 1
		z(dotu)
		z(dotc)
//		z(nrm2)
		{"nrm2", lux_cast((nrm2<complex<double>, double>))},
//		z(asum)
		{"asum", lux_cast((asum<complex<double>, double>))},
		z(iamax)
		z(swap)
		z(copy)
		z(axpy)
		z(scal)
		// LEVEL 2
		z(gemv)
		z(gbmv)
		z(trmv)
		z(tbmv)
		z(tpmv)
		z(trsv)
		z(tbsv)
		z(tpsv)
		z(hemv)
		z(hbmv)
		z(hpmv)
		z(geru)
		z(gerc)
//		z(her)
		{"her", lux_cast(her<double>)},
//		z(hpr)
		{"hpr", lux_cast(hpr<double>)},
		z(her2)
		z(hpr2)
		// LEVEL 3
		z(gemm)
		z(symm)
		z(hemm)
		z(syrk)
//		z(herk)
		{"herk", lux_cast(herk<double>)},
		z(syr2k)
//		z(her2k)
		{"her2k", lux_cast(her2k<double>)},
		z(trmm)
		z(trsm)
		// DONE
		{nullptr}
	};
	
	// Register functions in extant global metatables
	
	if (luaL_getmetatable(state, "float"))
	{
		luaL_setfuncs(state, sreg, 0);
		if (luaL_getsubtable(state, -1, "complex"))
		{
			luaL_setfuncs(state, creg, 0);
			lua_pop(state, 2);
		}
		else
		 return luaL_error(state, "require'complex' for float");
	}
	else
	 return luaL_error(state, "require'array' for float");
	 
	if (luaL_getmetatable(state, "double"))
	{
		luaL_setfuncs(state, dreg, 0);
		if (luaL_getsubtable(state, -1, "complex"))
		{
			luaL_setfuncs(state, zreg, 0);
			lua_pop(state, 2);
		}
		else
		 return luaL_error(state, "require'complex' for double");
	}
	else
	 return luaL_error(state, "require'array' for double");
	 
	// Done
	
	return 0;
}

