#include "simple_shell.h"

int myhistori(info_t *info);
int remv_alias(info_t *info, char *string);
int init_alias(info_t *info, char *string);
int puts_alias(list_t *node);
int alias_mem(info_t *info);
/**
 * myhistori - displays the history list, one command by line, preceded
 *              with line numbers, starting at 0.
 * @info: Structure containing potential arguments. Used to maintain
 *        a constant function prototype.
 *  Return: Always 0
 */
int myhistori(info_t *info)
{
	print_list(info->hist_node);
	return (0);
}

/**
 * remv_alias - unsets alias to string
 * @info: parameter struct
 * @string: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int remv_alias(info_t *info, char *string)
{
	char *p, c;
	int ret;

	p = StrChr(string, '=');
	if (!p)
		return (1);
	c = *p;
	*p = 0;
	ret = delete_node_at_index(&(info->alias_node),
		get_node_index(info->alias_node,
			node_starts_with(info->alias_node, string, -1)));
	*p = c;
	return (ret);
}

/**
 * init_alias - sets alias to string
 * @info: parameter struct
 * @string: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int init_alias(info_t *info, char *string)
{
	char *p;

	p = StrChr(string, '=');
	if (!p)
		return (1);
	if (!*++p)
		return (remv_alias(info, string));

	remv_alias(info, string);
	return (add_node_end(&(info->alias_node), string, 0) == NULL);
}

/**
 * puts_alias - prints an alias string
 * @node: the alias node
 *
 * Return: Always 0 on success, 1 on error
 */
int puts_alias(list_t *node)
{
	char *p = NULL, *a = NULL;

	if (node)
	{
		p = StrChr(node->string, '=');
		for (a = node->string; a <= p; a++)
			putchar(*a);
		put_char('\'');
		_write(p + 1);
		_write("'\n");
		return (0);
	}
	return (1);
}

/**
 * alias_mem - mimics the alias builtin (man alias)
 * @info: Structure containing potential arguments. Used to maintain
 *          a constant function prototype.
 *  Return: Always 0
 */
int alias_mem(info_t *info)
{
	int i = 0;
	char *p = NULL;
	list_t *node = NULL;

	if (info->argum_count == 1)
	{
		node = info->alias_node;
		while (node)
		{
			puts_alias(node);
			node = node->next_node;
		}
		return (0);
	}
	for (i = 1; info->argum_arr[i]; i++)
	{
		p = StrChr(info->argum_arr[i], '=');
		if (p)
			init_alias(info, info->argum_arr[i]);
		else
			puts_alias(node_starts_with(info->alias_node,
						info->argum_arr[i], '='));
	}

	return (0);
}
