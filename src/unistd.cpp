#include "lux.hpp"
#include <unistd.h>

#define REG(reg) {#reg, lux_cast(reg)},
#define ARG(arg) {#arg, arg},

extern "C" int luaopen_unistd(lua_State *state)
{
	luaL_Reg regs[] =
	{
	REG(access)
	REG(alarm)
	REG(chdir)
	REG(chown)
	REG(close)
	REG(confstr)
//	REG(crypt)
	REG(dup)
	REG(dup2)
	REG(_exit)
//	REG(encrypt)
//	REG(execl)
//	REG(execle)
//	REG(execlp)
//	REG(execv)
//	REG(execve)
//	REG(execvp)
	REG(faccessat)
	REG(fchdir)
	REG(fchown)
	REG(fchownat)
	REG(fdatasync)
//	REG(fexecve)
	REG(fork)
	REG(fpathconf)
	REG(sync)
	REG(ftruncate)
	REG(getcwd)
	REG(getegid)
	REG(geteuid)
	REG(getgid)
//	REG(getgroups)
	REG(gethostid)
	REG(gethostname)
	REG(getlogin)
	REG(getlogin_r)
//	REG(getopt)
	REG(getpgid)
	REG(getpgrp)
	REG(getpid)
	REG(getppid)
	REG(getsid)
	REG(getuid)
	REG(isatty)
	REG(lchown)
	REG(link)
	REG(linkat)
	REG(lockf)
	REG(lseek)
	REG(nice)
	REG(pathconf)
	REG(pause)
	REG(pipe)
	REG(pread)
	REG(pwrite)
	REG(read)
	REG(readlink)
	REG(readlinkat)
	REG(rmdir)
	REG(setegid)
	REG(seteuid)
	REG(setgid)
	REG(setpgid)
	REG(setpgrp)
	REG(setregid)
	REG(setreuid)
	REG(setsid)
	REG(setuid)
	REG(sleep)
	REG(swab)
	REG(symlink)
	REG(symlinkat)
	REG(sync)
	REG(sysconf)
	REG(tcgetpgrp)
	REG(tcsetpgrp)
	REG(truncate)
	REG(ttyname)
	REG(ttyname_r)
	REG(unlink)
	REG(unlinkat)
	REG(write)
	{nullptr}
	};
	luaL_newlib(state, regs);

	lux_Reg<lua_Integer> args[] =
	{
	// access
	ARG(R_OK)
	ARG(W_OK)
	ARG(X_OK)
	ARG(F_OK)
	// confstr
	ARG(_CS_PATH)
	// lseek/fcntl
	ARG(SEEK_SET)
	ARG(SEEK_CUR)
	ARG(SEEK_END)
	// sysconf
	ARG(_SC_ARG_MAX)
	ARG(_SC_ATEXIT_MAX)
	ARG(_SC_CHILD_MAX)
	// lockf
	ARG(F_LOCK)
	ARG(F_ULOCK)
	ARG(F_TEST)
	ARG(F_TLOCK)
	// pathconf
	ARG(_PC_ASYNC_IO)
	ARG(_PC_CHOWN_RESTRICTED)
	ARG(_PC_FILESIZEBITS)
	ARG(_PC_LINK_MAX)
	ARG(_PC_MAX_CANON)
	ARG(_PC_MAX_INPUT)
	ARG(_PC_NAME_MAX)
	ARG(_PC_NO_TRUNC)
	ARG(_PC_PATH_MAX)
	ARG(_PC_PIPE_BUF)
	ARG(_PC_PRIO_IO)
	ARG(_PC_SYNC_IO)
	ARG(_PC_VDISABLE)
	// std files
	ARG(STDERR_FILENO)
	ARG(STDIN_FILENO)
	ARG(STDOUT_FILENO)
	{nullptr}
	};
	lux_settable(state, args);

	return 1;
}
