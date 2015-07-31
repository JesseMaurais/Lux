#ifndef __lxchars__
#define __lxchars__

#include <cstdlib>
#include <cstring>
#include <wchar.h>
#include <uchar.h>

// Some compilers (GCC) lack the codecvt header for C++11

struct lux_Chars : mbstate_t
{
	lux_Chars(void)
	{
		reset();
	}

	// Reset the shift state

	void reset(void)
	{
		(void) memset(this, 0, sizeof(mbstate_t));
	}

	// Char length in bytes

	ssize_t charsize(const char *str, size_t max = MB_CUR_MAX)
	{
		return mbrlen(str, max, this);
	}

	// String length in chars

	ssize_t stringsize(const char *str, size_t max)
	{
		ssize_t sz;
		size_t n;
		for (n = 0; n < max and str[n]; n += sz)
		{
			sz = charsize(str + n, max - n);
			if (sz < 0) return sz;
		}
		return n;
	}

	// Char conversion for wide-char

	ssize_t tochar(char *dst, wchar_t src)
	{
		return wcrtomb(dst, src, this);
	}

	ssize_t tochar(wchar_t *dst, const char *src, size_t max = MB_CUR_MAX)
	{
		return mbrtowc(dst, src, max, this);
	}

	// Char conversion for UTF-16

	ssize_t tochar(char *dst, char16_t src)
	{
		return c16rtomb(dst, src, this);
	}

	ssize_t tochar(char16_t *dst, const char *src, size_t max = MB_CUR_MAX)
	{
		return mbrtoc16(dst, src, max, this);
	}

	// Char conversion for UTF-32

	ssize_t tochar(char *dst, char32_t src)
	{
		return c32rtomb(dst, src, this);
	}

	ssize_t tochar(char32_t *dst, const char *src, size_t max = MB_CUR_MAX)
	{
		return mbrtoc32(dst, src, max, this);
	}

	// String conversion

	template <class Char>
	ssize_t tostring(char *dst, const Char *src, size_t len)
	{
		char str[MB_CUR_MAX];
		ssize_t sz;
		size_t n;
		for (n = 0; n < len and src[n]; ++n)
		{
			sz = tochar(str, src[n]);
			if (sz < 0) return sz;
			strncpy(dst, str, sz);
			dst += sz;
		}
		return n;
	}

	template <class Char>
	ssize_t tostring(Char *dst, const char *src, size_t len)
	{
		ssize_t sz;
		size_t n;
		for (n = 0; n < len and src[n]; n += sz, dst++)
		{
			sz = tochar(dst, src + n, len - n);
			if (sz < 0) return sz;
		}
		return n;
	}

	// Generic 'array' conversion (any integral types)

	template <class User>
	ssize_t from(char *dst, const User *src, size_t len)
	{
		switch (sizeof(User))
		{
		default:
			return -4;
		case sizeof(char):
			strncpy(dst, (const char *) src, len);
			return len;
		case sizeof(char16_t):
			return tostring(dst, (const char16_t *) src, len);
		case sizeof(char32_t):
			return tostring(dst, (const char32_t *) src, len);
		}
	}

	template <class User>
	ssize_t to(User *dst, const char *src, size_t len)
	{
		switch (sizeof(User))
		{
		default:
			return -4;
		case sizeof(char):
			strncpy((char *) dst, src, len);
			return len;
		case sizeof(char16_t):
			return tostring((char16_t *) dst, src, len);
		case sizeof(char32_t):
			return tostring((char32_t *) dst, src, len);
		}
	}
};

#endif // file

