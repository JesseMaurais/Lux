#include <cstdlib>
#include "lux.hpp"


// Pretend this is the class/struct/union you already have.

struct Account
{
	double Balance;

	Account(void) : Balance(0)
	{
		puts("Opening account");
	}

	~Account(void)
	{
		puts("Closing account");
	}

	void Withdrawl(double amount)
	{
		printf("-$%.2f\n", amount);
		Balance -= amount;
	}

	void Deposit(double amount)
	{
		printf("+$%.2f\n", amount);
		Balance += amount;
	}
};


template <> luaL_Reg lux_Class<Account>::regs [] =
	{
	{"Withdrawl", lux_method(Account, Withdrawl)},
	{"Deposit", lux_method(Account, Deposit)},
	{"Balance", lux_member(Account, Balance)},
	{nullptr}
	};


// This is the interface you write.

extern "C" int luaopen_test(lua_State *state)
{
	luaL_requiref(state, "Account", lux_Class<Account>::open, true);
	return 1;
}

