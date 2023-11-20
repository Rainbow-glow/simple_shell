#include "simple_shell.h"

/**
 * _myhistory - displays the history list, one command by line, preceded
 *              with line numbers, starting at 0.
 * @info: Structure containing potential arguments. Used to maintain
 *        a constant function prototype.
 *  Return: Always 0
 */
int _myhistory(info_t *info)
{
	print_list(info->hist_node);
	return (0);
}

/**
 * unset_alias - sets alias to string
 * @info: parameter struct
 * @string: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int unset_alias(info_t *info, char *string)
{
	char *p, c;
	int ret;

	p = _strchr(string, '=');
	if (!p)
		return (1);
	c = *p;
	*p = 0;
	ret = delete_node_at_index(&(info->alias),
		get_node_index(info->alias, node_starts_with(info->alias, string, -1)));
	*p = c;
	return (ret);
}

/**
 * set_alias - sets alias to string
 * @info: parameter struct
 * @string: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int set_alias(info_t *info, char *string)
{
	char *p;

	p = _strchr(string, '=');
	if (!p)
		return (1);
	if (!*++p)
		return (unset_alias(info, string));

	unset_alias(info, string);
	return (add_node_end(&(info->alias), string, 0) == NULL);
}

/**
 * print_alias - prints an alias string
 * @node: the alias node
 *
 * Return: Always 0 on success, 1 on error
 */
int print_alias(list_t *node)
{
	char *p = NULL, *a = NULL;

	if (node)
	{
		p = _strchr(node->string, '=');
		for (a = node->string; a <= p; a++)
			_putchar(*a);
		_putchar('\'');
		_puts(p + 1);
		_puts("'\n");
		return (0);
	}
	return (1);
}

/**
 * _myalias - mimics the alias builtin (man alias)
 * @info: Structure containing potential arguments. Used to maintain
 *          a constant function prototype.
 *  Return: Always 0
 */
int _myalias(info_t *info)
{
	int i = 0;
	char *p = NULL;
	list_t *node = NULL;

	if (info->argum_count == 1)
	{
		node = info->alias;
		while (node)
		{
			print_alias(node);
			node = node->next_node;
		}
		return (0);
	}
	for (i = 1; info->argum_arr[i]; i++)
	{
		p = _strchr(info->argum_arr[i], '=');
		if (p)
			set_alias(info, info->argum_arr[i]);
		else
			print_alias(node_starts_with(info->alias, info->argum_arr[i], '='));
	}

	return (0);
}
