#include "simple_shell.h"

int exit_me(info_t *info);
int cd_me(info_t *info);
int helper_me(info_t *info);
/**
 * exit_me - exits the shell
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: exits with a given exit status
 *         (0) if info.argv[0] != "exit"
 */
int exit_me(info_t *info)
{
	int check;

	if (info->argv[1])  /* If there is an exit arguement */
	{
	`	check = _erratoi(info->argv[1]);
		if (check == -1)
		{
			info->status = 2;
			print_error(info, "Illegal number: ");
			_eputs(info->argv[1]);
			_eputchar('\n');
			return (1);
		}
		info->err_num = _erratoi(info->argv[1]);
		return (-2);
	}
	info->err_num = -1;
	return (-2);
}

/**
 * cd_me - changes the current directory of the process
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int cd_me(info_t *info)
{
	char *s, *dir, buffer[1024];
	int chdir_ret;

	s = getcwd(buffer, 1024);
	if (!s)
		_write("TODO: >>getcwd failure emsg here<<\n");
	if (!info->argv[1])
	{
		dir = pwd_env(info, "HOME=");
		if (!dir)
			chdir_ret = /* TODO: what should this be? */
				chdir((dir = pwd_env(info, "PWD=")) ? dir : "/");
		else
			chdir_ret = chdir(dir);
	}
	else if (StrCmp(info->argv[1], "-") == 0)
	{
		if (!pwd_env(info, "OLDPWD="))
		{
			_write(s);
			put_char('\n');
			return (1);
		}
		_write(pwd_env(info, "OLDPWD=")), put_char('\n');
		chdir_ret = /* TODO: what should this be? */
			chdir((dir = pwd_env(info, "OLDPWD=")) ? dir : "/");
	}
	else
		chdir_ret = chdir(info->argv[1]);
	if (chdir_ret == -1)
	{
		print_error(info, "cd failure ");
		_eputs(info->argv[1]), _eputchar('\n');
	}
	else
	{
		set_env(info, "OLDPWD", pwd_env(info, "PWD="));
		set_env(info, "PWD", getcwd(buffer, 1024));
	}
	return (0);
}

/**
 * helper_me - changes the current directory of the process
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int helper_me(info_t *info)
{
	char **arg_array;

	arg_array = info->argv;
	_write("help call works. Function not yet implemented \n");
	if (0)
		_write(*arg_array); /* temp att_unused workaround */
	return (0);
}
