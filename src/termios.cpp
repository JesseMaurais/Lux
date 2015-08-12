#include "lux.hpp"
#include <termios.h>

static int c_cc(lua_State *state)
{
	auto tc = lux_to<termios*>(state, 1);
	size_t cc = lux_to<size_t>(state, 2);
	luaL_argcheck(state, (cc < NCCS), 2, "cc < NCCS");
	lux_push(state, tc->c_cc[cc]);
	return 1;
}

template <> luaL_Reg lux_Class<termios>::index[] =
{
	{"c_iflag", lux_member(termios, c_iflag)},
	{"c_oflag", lux_member(termios, c_oflag)},
	{"c_cflag", lux_member(termios, c_cflag)},
	{"c_lflag", lux_member(termios, c_lflag)},
	{"c_cc", c_cc},
	{"makeraw", lux_cast(cfmakeraw)},
	{"getispeed", lux_cast(cfgetispeed)},
	{"getospeed", lux_cast(cfgetospeed)},
	{"setispeed", lux_cast(cfsetispeed)},
	{"setospeed", lux_cast(cfsetospeed)},
	{"setspeed", lux_cast(cfsetspeed)},
	{nullptr}
};

extern "C" int luaopen_termios(lua_State *state)
{
	luaL_requiref(state, "termios", lux_Class<termios>::open, false);

	luaL_Reg regs[] =
	{
	{"getsid", lux_cast(tcgetsid)},
	{"getattr", lux_cast(tcgetattr)},
	{"setattr", lux_cast(tcsetattr)},
	{"sendbreak", lux_cast(tcsendbreak)},
	{"drain", lux_cast(tcdrain)},
	{"flush", lux_cast(tcflush)},
	{"flow", lux_cast(tcflow)},
	{nullptr}
	};
	luaL_setfuncs(state, regs, 0);

	lux_Reg<lua_Integer> args[] =
	{
	// c_cc
	{"NCCS", NCCS},
	{"VEOF", VEOF},
	{"VEOL", VEOL},
	{"VERASE", VERASE},
	{"VINTR", VINTR},
	{"VKILL", VKILL},
	{"VQUIT", VQUIT},
	{"VSTART", VSTART},
	{"VSTOP", VSTOP},
	{"VSUSP", VSUSP},
	// c_iflag
	{"BRKINT", BRKINT},
	{"ICRNL", ICRNL},
	{"IGNBRK", IGNBRK},
	{"IGNCR", IGNCR},
	{"IGNPAR", IGNPAR},
	{"INLCR", INLCR},
	{"INPCK", INPCK},
	{"ISTRIP", ISTRIP},
	{"IUCLC", IUCLC},
	{"IXANY", IXANY},
	{"IXOFF", IXOFF},
	{"IXON", IXON},
	{"PARMRK", PARMRK},
	// c_oflag
	{"OPOST", OPOST},
//	{"OLCUC", OLCUC},
	{"ONLCR", ONLCR},
	{"OCRNL", OCRNL},
	{"ONOCR", ONOCR},
	{"ONLRET", ONLRET},
	{"OFILL", OFILL},
	{"NLDLY", NLDLY},
	{"NL0", NL0},
	{"NL1", NL1},
	{"CRDLY", CRDLY},
	{"CR0", CR0},
	{"CR1", CR1},
	{"CR2", CR2},
	{"CR3", CR3},
	{"TABDLY", TABDLY},
	{"TAB0", TAB0},
	{"TAB1", TAB1},
	{"TAB2", TAB2},
	{"TAB3", TAB3},
	{"BSDLY", BSDLY},
	{"BS0", BS0},
	{"BS1", BS1},
	{"VTDLY", VTDLY},
	{"VT0", VT0},
	{"VT1", VT1},
	{"FFDLY", FFDLY},
	{"FF0", FF0},
	{"FF1", FF1},
	// speed_t
	{"B0", B0},
	{"B50", B50},
	{"B75", B75},
	{"B110", B110},
	{"B134", B134},
	{"B150", B150},
	{"B200", B200},
	{"B300", B300},
	{"B600", B600},
	{"B1200", B1200},
	{"B1800", B1800},
	{"B2400", B2400},
	{"B4800", B4800},
	{"B9600", B9600},
	{"B19200", B19200},
	{"B38400", B38400},
	// c_cflag
	{"CSIZE", CSIZE},
	{"CS5", CS5},
	{"CS6", CS6},
	{"CS7", CS7},
	{"CS8", CS8},
	{"CSTOPB", CSTOPB},
	{"CREAD", CREAD},
	{"PARENB", PARENB},
	{"PARODD", PARODD},
	{"HUPCL", HUPCL},
	{"CLOCAL", CLOCAL},
	// c_lflag
	{"ECHO", ECHO},
	{"ECHOE", ECHOE},
	{"ECHOK", ECHOK},
	{"ECHONL", ECHONL},
	{"ICANON", ICANON},
	{"IEXTEN", IEXTEN},
	{"ISIG", ISIG},
	{"NOFLSH", NOFLSH},
	{"TOSTOP", TOSTOP},
//	{"XCASE", XCASE},
	// setattr
	{"SANOW", TCSANOW},
	{"SADRAIN", TCSADRAIN},
	{"SAFLUSH", TCSAFLUSH},
	// flush
	{"IFLUSH", TCIFLUSH},
	{"IOFLUSH", TCIOFLUSH},
	{"OFLUSH", TCOFLUSH},
	// flow
	{"IOFF", TCIOFF},
	{"ION", TCION},
	{"OOFF", TCOOFF},
	{"OON", TCOON},
	{nullptr}
	};
	lux_settable(state, args);

	return 1;
}

