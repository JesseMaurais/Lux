#ifndef __lxchars__
#define __lxchars__

#include <cassert>
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

	ssize_t fromchar(char *dst, wchar_t src)
	{
		return wcrtomb(dst, src, this);
	}

	ssize_t tochar(wchar_t *dst, const char *src, size_t max = MB_CUR_MAX)
	{
		return mbrtowc(dst, src, max, this);
	}

	// Char conversion for UTF-16

	ssize_t fromchar(char *dst, char16_t src)
	{
		return c16rtomb(dst, src, this);
	}

	ssize_t tochar(char16_t *dst, const char *src, size_t max = MB_CUR_MAX)
	{
		return mbrtoc16(dst, src, max, this);
	}

	// Char conversion for UTF-32

	ssize_t fromchar(char *dst, char32_t src)
	{
		return c32rtomb(dst, src, this);
	}

	ssize_t tochar(char32_t *dst, const char *src, size_t max = MB_CUR_MAX)
	{
		return mbrtoc32(dst, src, max, this);
	}

	// String conversion base case

	ssize_t fromstring(char *dst, const char *src, size_t len)
	{
		strncpy(dst, src, len);
		return len;
	}

	ssize_t tostring(char *dst, const char *src, size_t len)
	{
		strncpy(dst, src, len);
		return len;
	}

	// String conversion

	template <class Char>
	ssize_t fromstring(char *dst, const Char *src, size_t len)
	{
		char str[MB_CUR_MAX];
		ssize_t sz;
		size_t n, m;
		for (n = m = 0; n < len and src[n]; m += sz, ++n)
		{
			sz = fromchar(str, src[n]);
			if (sz < 0) return sz;
			fromstring(dst + m, str, sz);
		}
		return m;
	}

	template <class Char>
	ssize_t tostring(Char *dst, const char *src, size_t len)
	{
		ssize_t sz;
		size_t n, m;
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
	ssize_t from(char *dst, const User *src, size_t len)
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
	ssize_t to(User *dst, const char *src, size_t len)
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
	ssize_t fromcast(char *dst, const User *src, size_t len)
	{
		assert(sizeof(User) == sizeof(Char));
		return fromstring(dst, (const Char *) src, len);
	}

	template <class Char, class User>
	ssize_t tocast(User *dst, const char *src, size_t len)
	{
		assert(sizeof(User) == sizeof(Char));
		return tostring((Char *) dst, src, len);
	}

	// Copying integer to char arrays is safe but slower

	template <class Char, class User>
	ssize_t fromcopy(char *dst, const User *src, size_t len)
	{
		Char buf[len];
		for (int n = 0; n < len; ++n) buf[n] = src[n];
		return fromstring(dst, buf, len);
	}

	template <class Char, class User>
	ssize_t tocopy(User *dst, const char *src, size_t len)
	{
		Char buf[len];
		ssize_t sz = tostring(buf, src, len);
		for (int n = 0; n < sz; ++n) dst[n] = buf[n];
		return sz;
	}
};

// Copy floating point arrays to preserve numeric values

template <>
ssize_t lux_Chars::from<float>(char *dst, const float *src, size_t len)
{
	return fromcopy<char32_t>(dst, src, len);
}
template <>
ssize_t lux_Chars::to<float>(float *dst, const char *src, size_t len)
{
	return tocopy<char32_t>(dst, src, len);
}

template <>
ssize_t lux_Chars::from<double>(char *dst, const double *src, size_t len)
{
	return fromcopy<char32_t>(dst, src, len);
}
template <>
ssize_t lux_Chars::to<double>(double *dst, const char *src, size_t len)
{
	return tocopy<char32_t>(dst, src, len);
}

template <>
ssize_t lux_Chars::from<long double>(char *dst, const long double *src, size_t len)
{
	return fromcopy<char32_t>(dst, src, len);
}
template <>
ssize_t lux_Chars::to<long double>(long double *dst, const char *src, size_t len)
{
	return tocopy<char32_t>(dst, src, len);
}

#endif // file

