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
	info->argum = NULL;
	info->argum_arr = NULL;
	info->path_finder = NULL;
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
	if (info->argum)
	{
		info->argum_arr = str_to_wrds(info->argum, " \t");
		if (!info->argum_arr)
		{

			info->argum_arr = malloc(sizeof(char *) * 2);
			if (info->argum_arr)
			{
				info->argum_arr[0] = StrDup(info->argum);
				info->argum_arr[1] = NULL;
			}
		}
		for (i = 0; info->argum_arr && info->argum_arr[i]; i++)
			;
		info->argum_count = i;

		remv_alias(info);
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
	info->path_finder = NULL;
	if (all)
	{
		if (!info->cmd_buf)
			free(info->argum);
		if (info->env_copy)
			free_list(&(info->env_copy));
		if (info->hist_node)
			free_list(&(info->hist_node));
		if (info->alias_node)
			free_list(&(info->alias_node));
		ffree(info->environ);
			info->environ = NULL;
		bfree((void **)info->cmd_buf);
		if (info->readfd > 2)
			close(info->readfd);
		e_putchar(BUF_FLUSH);
	}
}
