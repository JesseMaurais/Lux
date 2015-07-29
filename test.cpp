/*
 * A dummy module to test various features of the Lux emitter.
 */

#include <stdexcept>
#include <cstdlib>
#include "lux.hpp"

// For testing the basic C function interface and argument passing

static void WriteOut(int one, int two, int three, int four, int five)
{
	printf("%d, %d, %d, %d, %d\n", one, two, three, four, five);
}

// For testing the forwarding of C++ exceptions

static void Except(const char *string)
{
	throw std::runtime_error(string);
}

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

// This is the interface you write.

template <> luaL_Reg lux_Class<Account>::index[] =
	{
	{"Withdrawl", lux_method(Account, Withdrawl)},
	{"Deposit", lux_method(Account, Deposit)},
	{"Balance", lux_member(Account, Balance)},
	{nullptr}
	};

// Module entry point

extern "C" int luaopen_test(lua_State *state)
{
	// Basic way to register a single function
	lua_register(state, "WriteOut", lux_cast(WriteOut));
	// Lua should catch and report errors given here
	lua_register(state, "Except", lux_cast(Except));
	// Prefered way to register a class or module
	luaL_requiref(state, "Account", lux_Class<Account>::open, true);
	return 1;
}

