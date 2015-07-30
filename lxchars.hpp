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
		memset(this, 0, sizeof(mbstate_t));
	}

	// Character length in bytes

	size_t len(const char *str, size_t max = MB_CUR_MAX)
	{
		return mbrlen(str, max, this);
	}

	// Char conversion for wide-char

	size_t to(char *dst, wchar_t src)
	{
		return wcrtomb(dst, src, this);
	}

	size_t to(wchar_t *dst, const char *src, size_t max = MB_CUR_MAX)
	{
		return mbrtowc(dst, src, max, this);
	}

	// Char conversion for UTF-16

	size_t to(char *dst, char16_t src)
	{
		return c16rtomb(dst, src, this);
	}

	size_t to(char16_t *dst, const char *src, size_t max = MB_CUR_MAX)
	{
		return mbrtoc16(dst, src, max, this);
	}

	// Char conversion for UTF-32

	size_t to(char *dst, char32_t src)
	{
		return c32rtomb(dst, src, this);
	}

	size_t to(char32_t *dst, const char *src, size_t max = MB_CUR_MAX)
	{
		return mbrtoc32(dst, src, max, this);
	}

	// String conversion

	template <class Char>
	size_t to(char *dst, const Char *src)
	{
		char str[MB_CUR_MAX];
		size_t size;
		while (*src)
		{
			size = to(str, dst);
			strncpy(dst, str, size);
			dst += size;
			src ++;
		}
	}

	template <class Char>
	size_t to(Char *dst, const char *src)
	{
		size_t size;
		while (*src)
		{
			size = to(dst, src);
			src += size;
			dst ++;
		}
	}
};


#endif // file

