#include "lux.hpp"
#include "lapacke.hpp"

extern "C" int luaopen_lapack(lua_State *state)
{
	// SINGLE PRECISION FLOAT

	#define s(reg) {#reg, lux_cast(reg<float>)},
	
	luaL_Reg sreg [] =
	{
		// LINEAR EQUATIONS
		s(gesv)
		s(gbsv)
		s(gtsv)
		s(posv)
		s(ppsv)
		s(pbsv)
		s(ptsv)
		s(sysv)
		s(spsv)
		// LINEAR LEAST SQUARES
		s(gels)
		s(gelsy)
		s(gelss)
		s(gelsd) 
		// GENERALIZED LINEAR LEAST SQUARES
		s(gglse)
		s(ggglm)
		// EIGENVALUES AND SINGULAR VALUES
		s(gees)
		s(geev)
		s(gesvd)
		s(syev)
		s(spev)
		s(sbev)
		s(stev)
		// GENERALIZED EIGENVALUES AND SINGULAR VALUES
		s(gges)
		s(ggev)
		s(ggsvd)
		s(sygv)
		s(spgv)
		s(sbgv)
		// COMPUTATIONAL ROUTINES FOR LINEAR EQUATIONS
		s(getrf)
		s(getrf)
		// END 
		{nullptr}
	};
	
	// DOUBLE PRECISION FLOAT

	#define d(reg) {#reg, lux_cast(reg<double>)},
	
	luaL_Reg dreg [] =
	{
		// LINEAR EQUATIONS
		d(gesv)
		d(gbsv)
		d(gtsv)
		d(posv)
		d(ppsv)
		d(pbsv)
		d(ptsv)
		d(sysv)
		d(spsv)
		// LINEAR LEAST SQUARES
		d(gels)
		d(gelsy)
		d(gelss)
		d(gelsd)
		// GENERALISED LINEAR LEAST SQUARES
		d(gglse)
		d(ggglm)
		// EIGENVALUES AND SINGULAR VALUES
		d(gees)
		d(geev)
		d(gesvd)
		d(syev)
		d(spev)
		d(sbev)
		d(stev)
		// GENERALIZED EIGENVALUES AND SINGULAR VALUES
		d(gges)
		d(ggev)
		d(ggsvd)
		d(sygv)
		d(spgv)
		d(sbgv)
		// COMPUTATIONAL ROUTINES FOR LINEAR EQUATIONS
		d(getrf)
		d(getrf)
		// END
		{nullptr}
	};
	
	// SINGLE PRECISION COMPLEX

	#define c(reg) {#reg, lux_cast(reg<complex<float>>)},
	
	luaL_Reg creg [] =
	{
		// LINEAR EQUATIONS
		c(gesv)
		c(gbsv)
		c(gtsv)
		c(posv)
		c(ppsv)
		c(pbsv)
//		c(ptsv)
		{"ptsv", lux_cast((ptsv<complex<float>, float>))},
		c(sysv)
		c(spsv)
		c(hesv)
		c(hpsv)
		// LINEAR LEAST SQUARES
		c(gels)
//		c(gelsy)
		{"gelsy", lux_cast((gelsy<complex<float>, float>))},
//		c(gelss)
		{"gelss", lux_cast((gelss<complex<float>, float>))},
//		c(gelsd)
		{"gelsd", lux_cast((gelsd<complex<float>, float>))},
		// GENERALISED LINEAR LEAST SQUARES
		c(gglse)
		c(ggglm)
		// EIGENVALUES AND SINGULAR VALUES
		c(gees)
		c(geev)
		c(gesvd)
//		c(heev)
		{"heev", lux_cast(heev<float>)},
//		c(hpev)
		{"hpev", lux_cast(hpev<float>)},
//		c(hbev)
		{"hbev", lux_cast(hbev<float>)},
		// GENERALIZED EIGENVALUES AND SINGULAR VALUES
		c(gges)
		c(ggev)
//		c(ggsvd)
		{"ggsvd", lux_cast((ggsvd<complex<float>, float>))},
//		c(hegv)
		{"hegv", lux_cast(hegv<float>)},
//		c(hpgv)
		{"hpgv", lux_cast(hpgv<float>)},
//		c(hbgv)
		{"hbgv", lux_cast(hbgv<float>)},
		// COMPUTATIONAL ROUTINES FOR LINEAR EQUATIONS
		c(getrf)
		c(getrf)
		// END		
		{nullptr}
	};
	
	// DOUBLE PRECISION COPMLEX

	#define z(reg) {#reg, lux_cast((reg<complex<double>>))},
	
	luaL_Reg zreg [] =
	{
		// LINEAR EQUATIONS
		z(gesv)
		z(gbsv)
		z(gtsv)
		z(posv)
		z(ppsv)
		z(pbsv)
//		z(ptsv)
		{"ptsv", lux_cast((ptsv<complex<double>, double>))},
		z(sysv)
		z(spsv)
		z(hesv)
		z(hpsv)
		// LINEAR LEAST SQUARES
		z(gels)
//		z(gelsy)
		{"gelsy", lux_cast((gelsy<complex<double>, double>))},
//		z(gelss)
		{"gelss", lux_cast((gelss<complex<double>, double>))},
//		z(gelsd)
		{"gelsd", lux_cast((gelsd<complex<double>, double>))},
		// GENERALISED LINEAR LEAST SQUARES
		z(gglse)
		z(ggglm)
		// EIGENVALUES AND SINGULAR VALUES
		z(gees)
		z(geev)
		z(gesvd)
//		z(heev)
		{"heev", lux_cast(heev<double>)},
//		z(hpev)
		{"hpev", lux_cast(hpev<double>)},
//		z(hbev)
		{"hbev", lux_cast(hbev<double>)},
		// GENERALIZED EIGENVALUES AND SINGULAR VALUES
		z(gges)
		z(ggev)
//		z(ggsvd)
		{"ggsvd", lux_cast((ggsvd<complex<double>, double>))},
//		z(hegv)
		{"hegv", lux_cast(hegv<double>)},
//		z(hpgv)
		{"hpgv", lux_cast(hpgv<double>)},
//		z(hbgv)
		{"hbgv", lux_cast(hbgv<double>)},
		// COMPUTATIONAL ROUTINES FOR LINEAR EQUATIONS
		z(getrf)
		z(getrf)
		// END		
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

