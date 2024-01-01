#include "simple_shell.h"

char *get_history_file(info_t *info);
int write_history(info_t *info);
int read_history(info_t *info);
int build_history_list(info_t *info, char *buf, int line_count);
int renumber_history(info_t *info);
/**
 * get_history_file - gets the history file
 * @info: parameter struct
 *
 * Return: allocated string containing history file
 */

char *get_history_file(info_t *info)
{
	char *buf, *dir;

	dir = gets_env(info, "HOME=");
	if (!dir)
		return (NULL);
	buf = malloc(sizeof(char) * (StrLen(dir) + StrLen(HIST_FILE) + 2));
	if (!buf)
		return (NULL);
	buf[0] = 0;
	StrCpy(buf, dir);
	StrCat(buf, "/");
	StrCat(buf, HIST_FILE);
	return (buf);
}

/**
 * write_history - creates a file, or appends to an existing file
 * @info: the parameter struct
 *
 * Return: 1 on success, else -1
 */
int write_history(info_t *info)
{
	ssize_t fd;
	char *filename = get_history_file(info);
	list_t *node = NULL;

	if (!filename)
		return (-1);

	fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
	free(filename);
	if (fd == -1)
		return (-1);
	for (node = info->hist_node; node; node = node->next_node)
	{
		_putsfd(node->string, fd);
		_putfd('\n', fd);
	}
	_putfd(BUF_FLUSH, fd);
	close(fd);
	return (1);
}

/**
 * read_history - reads history from file
 * @info: the parameter struct
 *
 * Return: histcount on success, 0 otherwise
 */
int read_history(info_t *info)
{
	int i, last = 0, line_count = 0;
	ssize_t fd, rd_len, file_size = 0;
	struct stat st;
	char *buf = NULL, *filename = get_history_file(info);

	if (!filename)
		return (0);

	fd = open(filename, O_RDONLY);
	free(filename);
	if (fd == -1)
		return (0);
	if (!fstat(fd, &st))
		file_size = st.st_size;
	if (file_size < 2)
		return (0);
	buf = malloc(sizeof(char) * (file_size + 1));
	if (!buf)
		return (0);
	rd_len = read(fd, buf, file_size);
	buf[file_size] = 0;
	if (rd_len <= 0)
		return (free(buf), 0);
	close(fd);
	for (i = 0; i < file_size; i++)
		if (buf[i] == '\n')
		{
			buf[i] = 0;
			build_history_list(info, buf + last, line_count++);
			last = i + 1;
		}
	if (last != i)
		build_history_list(info, buf + last, line_count++);
	free(buf);
	info->hist_count = line_count;
	while (info->hist_count-- >= HIST_MAX)
		delete_node_at_index(&(info->hist_node), 0);
	renumber_history(info);
	return (info->hist_count);
}

/**
 * build_history_list - adds entry to a history linked list
 * @info: Structure containing potential arguments. Used to maintain
 * @buf: buffer
 * @line_count: the history line count, histcount
 *
 * Return: Always 0
 */

int build_history_list(info_t *info, char *buf, int line_count)
{
	list_t *node = NULL;

	if (info->hist_node)
		node = info->hist_node;
	add_node_end(&node, buf, line_count);

	if (!info->hist_node)
		info->hist_node = node;
	return (0);
}

/**
 * renumber_history - renumbers the history linked list after changes
 * @info: Structure containing potential arguments. Used to maintain
 *
 * Return: the new histcount
 */
int renumber_history(info_t *info)
{
	list_t *node = info->hist_node;
	int i = 0;

	while (node)
	{
		node->find_num = i++;
		node = node->next_node;
	}
	return (info->hist_count = i);
}
