#include "simple_shell.h"

int pwd_env(info_t *info);
char *gets_env(info_t *info, const char *name);
int init_env(info_t *info);
int remv_env(info_t *info);
int add_env_list(info_t *info);

/**
 * pwd_env - prints the current environment
 * @info: Structure containing potential arguments. Used to maintain
 *          a constant function prototype.
 * Return: Always 0
 */
int pwd_env(info_t *info)
{
	print_list_str(info->env_copy);
	return (0);
}

/**
 * gets_env - gets the value of an environment variable
 * @info: Structure containing potential arguments. Used to maintain
 * @name: environment variable name
 *
 * Return: the value
 */
char *gets_env(info_t *info, const char *name)
{
	list_t *node = info->env_copy;
	char *p;

	while (node)
	{
		p = start_str(node->string, name);
		if (p && *p)
			return (p);
		node = node->next_node;
	}
	return (NULL);
}

/**
 * init_env - Initialize a new environment variable,
 *             or modify an existing one
 * @info: Structure containing potential arguments. Used to maintain
 *        a constant function prototype.
 *  Return: Always 0
 */
int init_env(info_t *info)
{
	if (info->argum_count != 3)
	{
		_eputs("Incorrect number of arguments\n");
		return (1);
	}
	if (set_env(info, info->argum_arr[1], info->argum_arr[2]))
		return (0);
	return (1);
}

/**
 * remv_env - Remove an environment variable
 * @info: Structure containing potential arguments. Used to maintain
 *        a constant function prototype.
 *  Return: Always 0
 */
int remv_env(info_t *info)

{
	int i;

	if (info->argum_count == 1)
	{
		_eputs("Too few arguments.\n");
		return (1);
	}
	for (i = 1; i <= info->argum_count; i++)
		del_env(info, info->argum_arr[i]);

	return (0);
}

/**
 * add_env_list - populates environment linked list
 * @info: Structure containing potential arguments. Used to maintain
 *          a constant function prototype.
 * Return: Always 0
 */
int add_env_list(info_t *info)
{
	list_t *node = NULL;
	size_t i;

	for (i = 0; environ[i]; i++)
		add_node_end(&node, environ[i], 0);
	info->env_copy = node;
	return (0);
}
