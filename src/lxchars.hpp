#ifndef __lxchars__
#define __lxchars__

/**
 * Support for the Unicode Transformation Format which is used to encode and
 * decode strings from the Universal Character Set, the standard way to
 * store and transmit text in various languages. Information on Unicode can
 * be found easily on the internet.  Noteworthy here is that we are using
 * the C11 functions for coding the multi byte sequence because many C++11
 * compilers are not equiped with the headers which contain the appropriate
 * classes. This is the current state of GCC.
 *
 * We are unconventionally storing the code points in arbitrary arrays
 * rather than just those of char, char16_t and char32_t. Safety is assured
 * by checks on the bit width of the base type and by defaulting to a copy
 * loop where the type is not a perfect match. 
 */

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <cwchar>
#include <cuchar>

struct lux_Chars : std::mbstate_t
{
	lux_Chars(void)
	{
		reset();
	}

	// Errors returned by C11 functions

	enum { NSEQUENCE = -1, NCOMPLETE = -2, SURROGATE = -3 };

	// Reset the shift state

	void reset(void)
	{
		std::memset(this, 0, sizeof(std::mbstate_t));
	}

	// Check if initial state

	bool isinit(void)
	{
		return std::mbsinit(this) != 0;
	}

	// Char length in bytes

	std::ptrdiff_t charsize(const char *str, std::size_t max = MB_CUR_MAX)
	{
		return std::mbrlen(str, max, this);
	}

	// String length in chars

	std::ptrdiff_t stringsize(const char *str, std::size_t max)
	{
		std::size_t m;
		for (auto n = m = 0; n < max and str[n]; ++m)
		{
			auto sz = charsize(str + n, max - n);
			if (sz < 0) return sz;
			n += sz;
		}
		return m;
	}

	// Char conversion for wide-char

	std::ptrdiff_t fromchar(char *dst, wchar_t src)
	{
		return std::wcrtomb(dst, src, this);
	}

	std::ptrdiff_t tochar(wchar_t *dst, const char *src, std::size_t max = MB_CUR_MAX)
	{
		return std::mbrtowc(dst, src, max, this);
	}

	// Char conversion for UTF-16

	std::ptrdiff_t fromchar(char *dst, char16_t src)
	{
		return std::c16rtomb(dst, src, this);
	}

	std::ptrdiff_t tochar(char16_t *dst, const char *src, std::size_t max = MB_CUR_MAX)
	{
		return std::mbrtoc16(dst, src, max, this);
	}

	// Char conversion for UTF-32

	std::ptrdiff_t fromchar(char *dst, char32_t src)
	{
		return std::c32rtomb(dst, src, this);
	}

	std::ptrdiff_t tochar(char32_t *dst, const char *src, std::size_t max = MB_CUR_MAX)
	{
		return std::mbrtoc32(dst, src, max, this);
	}

	// String conversion base case

	std::ptrdiff_t fromstring(char *dst, const char *src, std::size_t len)
	{
		std::strncpy(dst, src, len);
		return len;
	}

	std::ptrdiff_t tostring(char *dst, const char *src, std::size_t len)
	{
		std::strncpy(dst, src, len);
		return len;
	}

	// String conversion general

	template <class Char>
	std::ptrdiff_t fromstring(char *dst, const Char *src, std::size_t len)
	{
		std::size_t m;
		char buf[MB_CUR_MAX];
		for (auto n = m = 0; n < len and src[n]; ++n)
		{
			auto sz = fromchar(buf, src[n]);
			if (sz < 0) return sz;
			std::strncpy(dst + m, buf, sz);
			m += sz;
		}
		return m;
	}

	template <class Char>
	std::ptrdiff_t tostring(Char *dst, const char *src, std::size_t len)
	{
		std::size_t m;
		for (auto n = m = 0; n < len and src[n]; ++m)
		{
			auto sz = tochar(dst + m, src + n, len - n);
			if (sz < 0)
			{
				// Surrogate pairs recall at same src[n]
				if (SURROGATE == sz) continue;
				else return sz; // error
			}
			n += sz;
		}
		return m;
	}

	// Generic 'array' conversion for any integers

	template <class Char>
	std::ptrdiff_t from(char *dst, const Char *src, std::size_t len)
	{
		switch (sizeof(Char))
		{
		case sizeof(char):
			return fromcast<char>(dst, src, len);
		case sizeof(char16_t):
			return fromcast<char16_t>(dst, src, len);
		case sizeof(char32_t):
			return fromcast<char32_t>(dst, src, len);
		default:
			return fromcopy<char32_t>(dst, src, len);
		}
	}

	template <class Char>
	std::ptrdiff_t to(Char *dst, const char *src, std::size_t len)
	{
		switch (sizeof(Char))
		{
		case sizeof(char):
			return tocast<char>(dst, src, len);
		case sizeof(char16_t):
			return tocast<char16_t>(dst, src, len);
		case sizeof(char32_t):
			return tocast<char32_t>(dst, src, len);
		default:
			return tocopy<char32_t>(dst, src, len);
		}
	}

 private:

	// Casting integer to char arrays is not strictly type safe

	template <class Char, class User>
	std::ptrdiff_t fromcast(char *dst, const User *src, std::size_t len)
	{
		return fromstring(dst, (const Char *) src, len);
	}

	template <class Char, class User>
	std::ptrdiff_t tocast(User *dst, const char *src, std::size_t len)
	{
		return tostring((Char *) dst, src, len);
	}

	// Copying integer to char arrays is safe but slower

	template <class Char, class User>
	std::ptrdiff_t fromcopy(char *dst, const User *src, std::size_t len)
	{
		Char buf[len];
		for (decltype(len) n = 0; n < len; ++n) buf[n] = src[n];
		return fromstring(dst, buf, len);
	}

	template <class Char, class User>
	std::ptrdiff_t tocopy(User *dst, const char *src, std::size_t len)
	{
		Char buf[len];
		auto sz = tostring(buf, src, len);
		for (decltype(sz) n = 0; n < sz; ++n) dst[n] = buf[n];
		return sz;
	}
};

#include "lxerror.hpp"
#include "lxstore.hpp"
#include "lxstack.hpp"

template <class Char> struct lux_Coder
{
	typedef lux_Store<Char*> Type;

	/// Construct this array from a string
	static int decode(lua_State *state)
	{
		// Get UTF-8 encoded string and it's size in bytes
		std::size_t size;
		auto string = lua_tolstring(state, 1, &size);
		// Find the number of multibyte characters
		lux_Chars shift;
		auto length = shift.stringsize(string, size);
		// Check the string for encoding errors
		if (length < 0) return lux_argerror(state, 1);
		shift.reset();
		// Create array and convert
		auto data = new Char [length];
		size = shift.to(data, string, length);
		// Put the array on the stack
		Type::push(state, data, size);
		return 1;
	}

	/// Convert this array to a string
	static int encode(lua_State *state)
	{
		auto user = Type::check(state);
		// Pointers not supported
		auto size = std::abs(user->size);
		lux_argcheck(state, 1, 0 < size);
		// Store UTF-8 characters
		char data[size * MB_CUR_MAX];
		// Conversion
		lux_Chars shift;
		size = shift.from(data, user->data, size);
		if (size < 0) return lux_perror(state);
		// Push data as string onto stack
		lua_pushlstring(state, data, size);
		return 1;
	}

	/// Write as string to a file
	static int puts(lua_State *state)
	{
		auto user = Type::check(state, 1);
		// Pointers not supported
		auto size = std::abs(user->size);
		lux_argcheck(state, 1, 0 < size);
		// Stream that we will write to
		auto file = lux_opt(state, 2, stdout);
		// Store UTF-8 characters
		lux_Chars shift;
		char data[MB_CUR_MAX*size];
		size = shift.from(data, user->data, size);
		if (size < 0) return lux_perror(state);
		data[size] = '\0';
		// Using fputs on given file
		if (0 < std::fputs(data, file))
		{
			// Return the number of bytes
			lua_pushinteger(state, size);
			return 1;
		}
		return 0;
	}

	/// Read as string from a file
	static int gets(lua_State *state)
	{
		auto user = Type::check(state, 1);
		// Pointers not supported
		auto size = std::abs(user->size);
		lux_argcheck(state, 1, 0 < size);
		// Stream that we will read from
		auto file = lux_opt(state, 2, stdin);
		// Store UTF-8 characters
		char data[MB_CUR_MAX*size];
		// Using fgets on given file
		if (not std::fgets(data, sizeof(data), file))
		{
			return 1;
		}
		// Conversion
		lux_Chars shift;
		size = shift.to(user->data, data, sizeof(data));
		if (size < 0) return lux_perror(state);
		// Return the number of bytes
		lua_pushinteger(state, size);
		return 1;
	}

	static int open(lua_State *state)
	{
		auto name = lua_tostring(state, -1);
		if (luaL_getmetatable(state, name))
		{
			luaL_Reg index[] =
			{
			{"decode", decode},
			{"encode", encode},
			{"puts", puts},
			{"gets", gets},
			{nullptr}
			};
			luaL_setfuncs(state, index, 0);
			return 1;
		}
		return luaL_error(state, "%s not extant", name); 
	}
};


#endif // file

