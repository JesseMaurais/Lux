#ifndef __lxchars__
#define __lxchars__

/**
 * Support for the Unicode Transformation Format (UTF) which is used to encode
 * and decode strings from the Universal Character Set (UCS), the standard way
 * to store and transmit text in various languages. Information on Unicode can
 * be found easily on the internet. 
 *
 * Noteworthy here is that we are using the C11 functions for coding the multi
 * byte sequence because many C++11 compilers are not equiped with the headers
 * which contain the appropriate classes. This is the current state of G++ 
 *
 * Unconventionally, we are storing the code points in arbitrary arrays rather
 * than just those of char, char16_t and char32_t. Saftey is assured by checks
 * on the bit width of the base type and by defaulting to a copying loop where
 * the type is not a perfect match. 
 */

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <wchar.h>
#include <uchar.h>

struct lux_Chars : mbstate_t
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
		memset(this, 0, sizeof(mbstate_t));
	}

	// Check if initial state

	bool isinit(void)
	{
		return mbsinit(this) not_eq 0;
	}

	// Char length in bytes

	int charsize(const char *str, size_t max = MB_CUR_MAX)
	{
		return mbrlen(str, max, this);
	}

	// String length in chars

	int stringsize(const char *str, size_t max)
	{
		int sz, n;
		for (n = 0; n < max and str[n]; n += sz)
		{
			sz = charsize(str + n, max - n);
			if (sz < 0) return sz;
		}
		return n;
	}

	// Char conversion for wide-char

	int fromchar(char *dst, wchar_t src)
	{
		return wcrtomb(dst, src, this);
	}

	int tochar(wchar_t *dst, const char *src, size_t max = MB_CUR_MAX)
	{
		return mbrtowc(dst, src, max, this);
	}

	// Char conversion for UTF-16

	int fromchar(char *dst, char16_t src)
	{
		return c16rtomb(dst, src, this);
	}

	int tochar(char16_t *dst, const char *src, size_t max = MB_CUR_MAX)
	{
		return mbrtoc16(dst, src, max, this);
	}

	// Char conversion for UTF-32

	int fromchar(char *dst, char32_t src)
	{
		return c32rtomb(dst, src, this);
	}

	int tochar(char32_t *dst, const char *src, size_t max = MB_CUR_MAX)
	{
		return mbrtoc32(dst, src, max, this);
	}

	// String conversion base case

	int fromstring(char *dst, const char *src, size_t len)
	{
		strncpy(dst, src, len);
		return len;
	}

	int tostring(char *dst, const char *src, size_t len)
	{
		strncpy(dst, src, len);
		return len;
	}

	// String conversion

	template <class Char>
	int fromstring(char *dst, const Char *src, size_t len)
	{
		char str[MB_CUR_MAX];
		int sz, n, m;
		for (n = m = 0; n < len and src[n]; m += sz, ++n)
		{
			sz = fromchar(str, src[n]);
			if (sz < 0) return sz;
			fromstring(dst + m, str, sz);
		}
		return m;
	}

	template <class Char>
	int tostring(Char *dst, const char *src, size_t len)
	{
		int sz, n, m;
		for (n = m = 0; n < len and src[n]; n += sz, ++m)
		{
			sz = tochar(dst + m, src + n, len - n);
			// Surrogate pairs recall at same src[n]
			if (sz < 0) if (sz == SURROGATE) sz = 0;
			else return sz; // Otherwise an error
		}
		return m;
	}

	// Generic 'array' conversion for any integers

	template <class User>
	int from(char *dst, const User *src, size_t len)
	{
		switch (sizeof(User))
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

	template <class User>
	int to(User *dst, const char *src, size_t len)
	{
		switch (sizeof(User))
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
	int fromcast(char *dst, const User *src, size_t len)
	{
		return fromstring(dst, (const Char *) src, len);
	}

	template <class Char, class User>
	int tocast(User *dst, const char *src, size_t len)
	{
		return tostring((Char *) dst, src, len);
	}

	// Copying integer to char arrays is safe but slower

	template <class Char, class User>
	int fromcopy(char *dst, const User *src, size_t len)
	{
		Char buf[len];
		for (int n = 0; n < len; ++n) buf[n] = src[n];
		return fromstring(dst, buf, len);
	}

	template <class Char, class User>
	int tocopy(User *dst, const char *src, size_t len)
	{
		Char buf[len];
		int sz = tostring(buf, src, len);
		for (int n = 0; n < sz; ++n) dst[n] = buf[n];
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
		size_t size;
		lux_Chars shift;
		// Get UTF-8 encoded string and it's size in bytes
		const char *string = lua_tolstring(state, 1, &size);
		// Find the number of multibyte characters
		int length = shift.stringsize(string, size);
		// Check the string for encoding errors
		if (length < 0) return lux_argerror(state, 1);
		else shift.reset();
		// Create array and convert
		Char *data = new Char [length];
		size = shift.to(data, string, length);
		// Put the array on the stack
		Type::push(state, data, size);
		return 1;
	}

	/// Convert this array to a string
	static int encode(lua_State *state)
	{
		Type *user = Type::check(state);
		// Pointers not supported
		int size = abs(user->size);
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
		Type *user = Type::check(state, 1);
		// Pointers not supported
		int size = abs(user->size);
		lux_argcheck(state, 1, 0 < size);
		// Stream that we will write to
		FILE *file = lux_opt(state, 2, stdout);
		// Store UTF-8 characters
		char data[size * MB_CUR_MAX];
		// Conversion
		lux_Chars shift;
		size = shift.from(data, user->data, size);
		if (size < 0) return lux_perror(state);
		// Using fputs on given file
		data[size] = '\0';
		fputs(data, file);
		// Return the number of bytes
		lua_pushinteger(state, size);
		return 1;
	}

	/// Read as string from a file
	static int gets(lua_State *state)
	{
		Type *user = Type::check(state, 1);
		// Pointers not supported
		int size = abs(user->size);
		lux_argcheck(state, 1, 0 < size);
		// Stream that we will read from
		FILE *file = lux_opt(state, 2, stdin);
		// Store UTF-8 characters
		char data[size * MB_CUR_MAX];
		// Using fgets on given file
		fgets(data, sizeof(data), file);
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

