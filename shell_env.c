#include "simple_shell.h"

char **get_env(info_t *info);
int del_env(info_t *info, char *var);
int set_env(info_t *info, char *var, char *vad);

/**
 * get_env - returns the string array copy of our environ
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
char **get_env(info_t *info)
{
	if (!info->environ || info->changed)
	{
		info->environ = list_to_strings(info->env_copy);
		info->changed = 0;
	}

	return (info->environ);
}

/**
 * del_env - Remove an environment variable
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: 1 on delete, 0 otherwise
 * @var: the string env var property
 */
int del_env(info_t *info, char *var)
{
	list_t *node = info->env_copy;
	size_t i = 0;
	char *p;

	if (!node || !var)
		return (0);

	while (node)
	{
		p = start_str(node->string, var);
		if (p && *p == '=')
		{
			info->changed = delete_node_at_index(&(info->env_copy), i);
			i = 0;
			node = info->env_copy;
			continue;
		}
		node = node->next_node;
		i++;
	}
	return (info->changed);
}

/**
 * set_env - Initialize a new environment variable,
 *             or modify an existing one
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * @var: the string env var property
 * @vad: the string env var value
 *  Return: Always 0
 */
int set_env(info_t *info, char *var, char *vad)
{
	char *buf = NULL;
	list_t *node;
	char *p;

	if (!var || !vad)
		return (0);

	buf = malloc(StrLen(var) + StrLen(vad) + 2);
	if (!buf)
		return (1);
	StrCpy(buf, var);
	StrCat(buf, "=");
	StrCat(buf, vad);
	node = info->env_copy;
	while (node)
	{
		p = start_str(node->string, var);
		if (p && *p == '=')
		{
			free(node->string);
			node->string = buf;
			info->changed = 1;
			return (0);
		}
		node = node->next_node;
	}
	add_node_end(&(info->env_copy), buf, 0);
	free(buf);
	info->changed = 1;
	return (0);
}
