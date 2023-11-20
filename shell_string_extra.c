#include "simple_shell.h"

char *StrnCpy(char *dest, char *src, int index);
char *StrnCat(char *dest, char *src, int index);
char *StrChr(char *s, char c);

/**
 * *StrnCpy - copies a string
 * @dest: the destination string to be copied to
 * @src: the source string
 * @index: the amount of characters to be copied
 * Return: the concatenated string
 */
char *StrnCpy(char *dest, char *src, int index)
{
	int i, j;
	char *s = dest;

	i = 0;
	while (src[i] != '\0' && i < index - 1)
	{
		dest[i] = src[i];
		i++;
	}
	if (i < index)
	{
		j = i;
		while (j < index)
		{
			dest[j] = '\0';
			j++;
		}
	}
	return (s);
}

/**
 * *StrnCat - concatenates two strings
 * @dest: the first string
 * @src: the second string
 * @index: the amount of bytes to be maximally used
 * Return: the concatenated string
 */
char *StrnCat(char *dest, char *src, int index)
{
	int i, j;
	char *s = dest;

	i = 0;
	j = 0;
	while (dest[i] != '\0')
		i++;
	while (src[j] != '\0' && j < index)
	{
		dest[i] = src[j];
		i++;
		j++;
	}
	if (j < index)
		dest[i] = '\0';
	return (s);
}

/**
 * *StrChr - locates a character in a string
 * @s: the string to be searched
 * @c: the character to look for
 * Return: (s) a pointer to the memory area s
 */
char *StrChr(char *s, char c)
{
	do {
		if (*s == c)
			return (s);
	} while (*s++ != '\0');

	return (NULL);
}
