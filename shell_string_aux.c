#include "simple_shell.h"

char *StrCpy(char *dest, char *src);
char *StrDup(const char *str);
void _write(char *);
int put_char(char c);
/**
 * StrCpy - copies a string
 * @dest: the destination
 * @src: the source
 *
 * Return: pointer to destination
 */
char *StrCpy(char *dest, char *src)
{
	int a = 0;

	if (dest == src || src == 0)
		return (dest);
	while (src[a])
	{
		dest[a] = src[a];
		a++;
	}
	dest[a] = 0;
	return (dest);
}

/**
 * StrDup - duplicates a string
 * @str: the string to duplicate
 *
 * Return: pointer to the duplicated string
 */
char *StrDup(const char *str)
{
	int len = 0;
	char *output;

	if (str == NULL)
		return (NULL);
	while (*str++)
		len++;
	output = malloc(sizeof(char) * (len + 1));
	if (!output)
		return (NULL);
	for (len++; len--;)
		output[len] = *--str;
	return (output);
}

/**
 *_write - prints an input string
 *@str: the string to be printed
 *
 * Return: Nothing
 */
void _write(char *str)
{
	int i = 0;

	if (!str)
		return;
	while (str[i] != '\0')
	{
		put_char(str[i]);
		i++;
	}
}

/**
 * put_char - writes the character c to stdout
 * @c: The character to print
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int put_char(char c)
{
	static int i;
	static char buf[WRITE_BUF_SIZE];

	if (c == BUF_FLUSH || i >= WRITE_BUF_SIZE)
	{
		write(1, buf, i);
		i = 0;
	}
	if (c != BUF_FLUSH)
		buf[i++] = c;
	return (1);
}
