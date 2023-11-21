#include "simple_shell.h"

void clear_info(info_t *info);
void set_info(info_t *info, char **av);
void free_info(info_t *info, int all);

/**
 * clear_info - initializes info_t struct
 * @info: struct address
 */
void clear_info(info_t *info)
{
	info->arg = NULL;
	info->argum_arr = NULL;
	info->path = NULL;
	info->argum_count = 0;
}

/**
 * set_info - initializes info_t struct
 * @info: struct address
 * @av: argument vector
 */
void set_info(info_t *info, char **av)
{
	int i = 0;

	info->file_name = av[0];
	if (info->arg)
	{
		info->argum_arr = strtow(info->arg, " \t");
		if (!info->argum_arr)
		{

			info->argum_arr = malloc(sizeof(char *) * 2);
			if (info->argum_arr)
			{
				info->argum_arr[0] = StrDup(info->arg);
				info->argum_arr[1] = NULL;
			}
		}
		for (i = 0; info->argum_arr && info->argum_arr[i]; i++)
			;
		info->argum_count = i;

		replace_alias(info);
		replace_vars(info);
	}
}

/**
 * free_info - frees info_t struct fields
 * @info: struct address
 * @all: true if freeing all fields
 */
void free_info(info_t *info, int all)
{
	ffree(info->argum_arr);
	info->argum_arr = NULL;
	info->path = NULL;
	if (all)
	{
		if (!info->cmd_buf)
			free(info->arg);
		if (info->env_copy)
			free_list(&(info->env_copy));
		if (info->history)
			free_list(&(info->history));
		if (info->alias)
			free_list(&(info->alias));
		ffree(info->environs);
			info->environs = NULL;
		bfree((void **)info->cmd_buf);
		if (info->readfd > 2)
			close(info->readfd);
		e_putchar(BUF_FLUSH);
	}
}
