#include "lux.hpp"
#include <cstdio>

#define REG(fun) {#fun, lux_cast(fun)},

extern "C" int luaopen_stdio(lua_State *state)
{
	luaL_Reg regs[] =
	{
	REG(clearerr)
	REG(ctermid)
//	REG(cuserid)
	REG(fclose)
	REG(fdopen)
	REG(feof)
	REG(ferror)
	REG(fflush)
	REG(fgetc)
	REG(fgetpos)
	REG(fgets)
	REG(fileno)
	REG(flockfile)
	REG(fopen)
//	REG(fprintf)
	REG(fputc)
	REG(fputs)
	REG(fread)
	REG(freopen)
//	REG(fscanf)
	REG(fseek)
	REG(fseeko)
	REG(fsetpos)
	REG(ftell)
	REG(ftello)
	REG(ftrylockfile)
	REG(funlockfile)
	REG(fwrite)
	REG(getc)
	REG(getchar)
//	REG(getc_unlocked)
//	REG(getchar_unlocked)
//	REG(getopt)
//	REG(gets)
	REG(getw)
	REG(pclose)
	REG(perror)
	REG(popen)
//	REG(printf)
	REG(putc)
	REG(putchar)
//	REG(putc_unlocked)
//	REG(putchar_unlocked)
	REG(puts)
	REG(putw)
	REG(remove)
	REG(rename)
	REG(rewind)
//	REG(scanf)
	REG(setbuf)
	REG(setvbuf)
//	REG(snprintf)
//	REG(sprintf)
//	REG(sscanf)
//	REG(tempnam)
	REG(tmpfile)
//	REG(tmpnam)
	REG(ungetc)
//	REG(vfprintf)
//	REG(vprintf)
//	REG(vsnprintf)
//	REG(vsprintf)
	{nullptr}
	};
	luaL_newlib(state, regs);

	lux_Reg<lua_Integer> args[] =
	{
	{"FILENAME_MAX", FILENAME_MAX},
	{"FOPEN_MAX", FOPEN_MAX},
	{"BUFSIZ", BUFSIZ},
	{"EOF", EOF},
	{nullptr}
	};
	lux_settable(state, args);

	lux_Reg<FILE*> ios[] =
	{
	{"stderr", stderr},
	{"stdin", stdin},
	{"stdout", stdout},
	{nullptr}
	};
	lux_settable(state, ios);

	return 1;
}

