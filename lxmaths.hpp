#ifndef __lxmaths__
#define __lxmaths__

#include "lxstack.hpp"
#include "lxtable.hpp"
#include <complex>
#include <cmath>

template <class Base> using complex<Base> = std::complex<Base>;

template <class Base> inline
void lux_push<complex<Base>>(lua_State *state, complex<Base> value)
{
	lua_newtable(state);
	lux_Table table(state);
	table<Base>("real", value.real());
	table<Base>("imag", value.imag());
}

template <class Base> inline
complex<Base> lux_opt<complex<Base>>(lua_State *state, int stack, complex<Base> opt)
{
	if (lua_istable(state, stack))
	{
		lux_Table table(state, stack);
		opt.real(table<Base>("real"));
		opt.imag(table<Base>("imag"));
	}
	return opt;
}

template <class Base> inline
complex<Base> lux_to<complex<Base>>(lua_State *state, int stack)
{
	lux_Table tab(state, stack);
	return complex<Base>(table<Base>("real"), table<Base>("imag"));
}


#endif // file
