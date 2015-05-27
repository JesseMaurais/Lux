#include <cstdlib>
#include "lux.hpp"


// Pretend this is the class you already have.

struct Account
{
	Account(void) : balance(0)
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
		balance -= amount;
	}

	void Deposit(double amount)
	{
		printf("+$%.2f\n", amount);
		balance += amount;
	}

	void Print(void)
	{
		printf("$%.2f\n", balance);
	}

 private:

	double balance;
};


// This is the interface you write.

template <> const char *lux_Class<Account>::Type::name = "Account";

template <> void lux_Class<Account>::setfuncs(lua_State *state)
{
	luaL_Reg regs [] =
	{
	 {"Withdrawl", lux_cast(Account::Withdrawl)},
	 {"Deposit", lux_cast(Account::Deposit)},
	 {"Print", lux_cast(Account::Print)},
	 {nullptr, nullptr}
	};
	luaL_setfuncs(state, regs, 0);
}


// You must add it like you would a library.

int main(int argc, char **argv)
{
	lua_State *state = luaL_newstate();
	luaL_openlibs(state);
	lux_Class<Account>::open(state);
	luaL_dofile(state, "test4.lua");
	lux_stackdump(state);
	lua_close(state);
}

