#include "lux.hpp"
#include <cstdio>

#define REG(fun) {#fun, lux_cast(fun)},

extern "C" int luaopen_cstdio(lua_State *state)
{
	luaL_Reg regs [] =
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
	// Standard FILE Streams
	lux_Type<FILE*>::name = "FILE";
	luaL_newmetatable(state, "FILE");
	lua_pop(state, 1);
	lux_push(state, stderr);
	lua_setfield(state, -2, "stderr");
	luaL_setmetatable(state, "FILE");
	lux_push(state, stdin);
	lua_setfield(state, -2, "stdin");
	luaL_setmetatable(state, "FILE");
	lux_push(state, stdout);
	lua_setfield(state, -2, "stdout");
	luaL_setmetatable(state, "FILE");
	// Constants
	lux_push(state, FILENAME_MAX);
	lua_setfield(state, -2, "FILENAME_MAX");
	lux_push(state, FOPEN_MAX);
	lua_setfield(state, -2, "FOPEN_MAX");
	lux_push(state, BUFSIZ);
	lua_setfield(state, -2, "BUFSIZ");
	lux_push(state, EOF);
	lua_setfield(state, -2, "EOF");
	// table
	return 1;
}

