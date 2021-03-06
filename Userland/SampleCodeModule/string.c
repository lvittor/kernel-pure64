// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <string.h>

int8_t strcmp(const char *p1, const char *p2)
{
	const unsigned char *s1 = (const unsigned char *)p1;
	const unsigned char *s2 = (const unsigned char *)p2;
	unsigned char c1, c2;
	do {
		c1 = (unsigned char)*s1++;
		c2 = (unsigned char)*s2++;
		if (c1 == '\0')
			return c1 - c2;
	} while (c1 == c2);
	return c1 - c2;
}

uint8_t strlen(const char *str)
{
	const char *s;
	for (s = str; *s; ++s)
		;
	return (s - str);
}

int strdiv(char *str, char **other, char token)
{
	while (*str) {
		if (*str == token) {
			*str = '\0';
			*other = str + 1;
			return 0;
		}
		str++;
	}
	return -1;
}

char *strcat(char *dest, const char *src)
{
	char *rdest = dest;

	while (*dest)
		dest++;
	while ((*dest++ = *src++))
		;
	return rdest;
}

void strcopy(char *dest, char *src)
{
	while (*src) {
		*dest = *src;
		dest++;
		src++;
	}
	*dest = 0;
}
