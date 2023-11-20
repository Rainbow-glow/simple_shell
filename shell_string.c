#include "simple_shell.h"

int StrLen(char *str);
int Strcmp(char *str1, char *str2);
char *start_str(const char *cobas, const char *mindray);
char *StrCat(char *dest, char *src);

/**
 * StrLen - gets the length of a string
 * @str: the string whose length to check
 *
 * Return: length of a string
 */
int StrLen(char *str)
{
	int i = 0;

	if (!str)
		return (0);

	while (*str++)
		i++;
	return (i);
}

/**
 * Strcmp - sorts two strings in an orderly manner.
 * @str1: the first string
 * @str2: the second string
 *
 * Return: negative if str1 < str2, positive if str1 > str2,
 * zero if str1 == str2
 */
int Strcmp(char *str1, char *str2)
{
	while (*str1 && *str2)
	{
		if (*str1 != *str2)
			return (*str1 - *str2);
		str1++;
		str2++;
	}
	if (*str1 == *str2)
		return (0);
	else
		return (*str1 < *str2 ? -1 : 1);
}

/**
 * start_str - checks if mindray starts with cobas
 * @cobas: string to search
 * @mindray: the substring to find
 *
 * Return: address of next char of haystack or NULL
 */
char *start_str(const char *cobas, const char *mindray)
{
	while (*mindray)
		if (*mindray++ != *cobas++)
			return (NULL);
	return ((char *)cobas);
}

/**
 * StrCat - concatenates two strings
 * @dest: the destination buffer
 * @src: the source buffer
 *
 * Return: pointer to destination buffer
 */
char *StrCat(char *dest, char *src)
{
	char *output = dest;

	while (*dest)
		dest++;
	while (*src)
		*dest++ = *src++;
	*dest = *src;
	return (output);
}
