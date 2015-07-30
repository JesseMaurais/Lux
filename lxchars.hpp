#ifndef __lxchars__
#define __lxchars__

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

	// Reset the shift state

	void reset(void)
	{
		(void) memset(this, 0, sizeof(mbstate_t));
	}

	// Character length in bytes

	ssize_t length(const char *str, size_t max = MB_CUR_MAX)
	{
		return mbrlen(str, max, this);
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
		for (n = 0; n < len && src[n]; n += sz, dst++)
		{
			sz = tochar(dst, src + n, len - n);
			if (sz < 0) return sz;
		}
		return n;
	}

	// Generic string conversion

	template <class Char>
	ssize_t to(char *dst, const Char *src, size_t len)
	{
		return _to<Char, sizeof(Char)>(dst, src, len);
	}

	template <class Char>
	ssize_t to(Char *dst, const char *src, size_t len)
	{
		return _to<Char, sizeof(Char)>(dst, src, len);
	}

 private:

	// Generic string conversion with char class size

	template <class Char, size_t Size>
	ssize_t _to(char *dst, const Char *src, size_t len);

	template <class Char, size_t Size>
	ssize_t _to(Char *dst, const char *src, size_t len);
};

// Generic 16-bit string conversion

template <class Char>
ssize_t lux_Chars::_to<Char, sizeof(char16_t)>(char *dst, const Char *src, size_t len)
{
	return lux_Chars::tostring(dst, (const char16_t *) src, len);
}

template <class Char>
ssize_t lux_Chars::_to<Char, sizeof(char16_t)>(Char *dst, const char *src, size_t len)
{
	return lux_Chars::tostring((char16_t *) dst, src, len);
}

// Generic 32-bit string conversion

template <class Char>
ssize_t lux_Chars::_to<Char, sizeof(char32_t)>(char *dst, const Char *src, size_t len)
{
	return lux_Chars::tostring(dst, (const char32_t *) src, len);
}

template <class Char>
ssize_t lux_Chars::_to<Char, sizeof(char32_t)>(Char *dst, const char *src, size_t len)
{
	return lux_Chars::tostring((char32_t *) dst, str, len);
}

#endif // file

