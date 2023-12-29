#include "simple_shell.h"

int loop_shell(info_t *info, char **av);
int find_builtin(info_t *info);
void find_command(info_t *info);
void fork_command(info_t *info);
/**
 * loop_shell - main shell loop
 * @info: the parameter & return info struct
 * @av: the argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
int loop_shell(info_t *info, char **av)
{
	ssize_t r = 0;
	int builtin_ret = 0;

	while (r != -1 && builtin_ret != -2)
	{
		clear_info(info);
		if (inter_active(info))
			puts("$ ");
		putchar(BUF_FLUSH);
		r = get_new_buf(info);
		if (r != -1)
		{
			set_info(info, av);
			builtin_ret = find_builtin(info);
			if (builtin_ret == -1)
				find_command(info);
		}
		else if (inter_active(info))
			putchar('\n');
		free_info(info, 0);
	}
	write_history(info);
	free_info(info, 1);
	if (!inter_active(info) && info->status)
		exit(info->status);
	if (builtin_ret == -2)
	{
		if (info->error_code == -1)
			exit(info->status);
		exit(info->error_code);
	}
	return (builtin_ret);
}

/**
 * find_builtin - finds a builtin command
 * @info: the parameter & return info struct
 *
 * Return: -1 if builtin not found,
 *			0 if builtin executed successfully,
 *			1 if builtin found but not successful,
 *			-2 if builtin signals exit()
 */
int find_builtin(info_t *info)
{
	int i, built_in_ret = -1;
	builtin_table builtintbl[] = {
		{"exit", exit_me},
		{"env", pwd_env},
		{"help", helper_me},
		{"history", my_histori},
		{"setenv", my_setenv},
		{"unsetenv", my_unsetenv},
		{"cd", cd_me},
		{"alias", alias_mem},
		{NULL, NULL}
	};

	for (i = 0; builtintbl[i].type; i++)
		if (Strcmp(info->argum_arr[0], builtintbl[i].type) == 0)
		{
			info->line_count++;
			built_in_ret = builtintbl[i].func(info);
			break;
		}
	return (built_in_ret);
}

/**
 * find_command - finds a command in PATH
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void find_command(info_t *info)
{
	char *path = NULL;
	int i, k;

	info->path_finder = info->argum_arr[0];
	if (info->line_count_flag == 1)
	{
		info->line_count++;
		info->line_count_flag = 0;
	}
	for (i = 0, k = 0; info->argum[i]; i++)
		if (!_isdel(info->argum[i], " \t\n"))
			k++;
	if (!k)
		return;

	path = find_pathstr(info, gets_env(info, "PATH="), info->argum_arr[0]);
	if (path)
	{
		info->path_finder = path;
		fork_command(info);
	}
	else
	{
		if ((inter_active(info) || gets_env(info, "PATH=")
			|| info->argum_arr[0][0] == '/') && exe_c(info, info->argum_arr[0]))
			fork_command(info);
		else if (*(info->argim) != '\n')
		{
			info->status = 127;
			print_error(info, "not found\n");
		}
	}
}

/**
 * fork_command - forks an exec thread to run command
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void fork_command(info_t *info)
{
	pid_t child_pid;

	child_pid = fork();
	if (child_pid == -1)
	{
		/* TODO: PUT ERROR FUNCTION */
		perror("Error:");
		return;
	}
	if (child_pid == 0)
	{
		if (execve(info->path_finder, info->argum_arr, get_env(info)) == -1)
		{
			free_info(info, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
		/* TODO: PUT ERROR FUNCTION */
	}
	else
	{
		wait(&(info->status));
		if (WIFEXITED(info->status))
		{
			info->status = WEXITSTATUS(info->status);
			if (info->status == 126)
				print_error(info, "Permission denied\n");
		}
	}
}
