#ifndef _SIMPLE_SHELL_H_
#define _SIMPLE_SHELL_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/* for read/write buffers */
#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

/* for command chaining */
#define CMD_NORM	0
#define CMD_OR		1
#define CMD_AND		2
#define CMD_CHAIN	3

/* for convert_number() */
#define CONVERT_LOWERCASE	1
#define CONVERT_UNSIGNED	2

/* 1 if using system getline() */
#define USE_GETLINE 0
#define USE_STRTOK 0

#define HIST_FILE	".simple_shell_history"
#define HIST_MAX	4096

extern char **environ;

/**
 * struct liststr - singly linked list
 * @find_num: the number field
 * @string: a string
 * @next_node: points to the next node
 */
typedef struct liststr
{
	int find_num;
	char *string;
	struct liststr *next_node;
} list_t;

/**
 *struct passinfo - contains pseudo-arguments to pass into a function,
 *		allowing a uniform prototype for the function pointer struct
 *@argum: a string generated from getline containing arguments
 *@argum_arr: an array of strings generated from argum
 *@path_finder: a string path for the current command
 *@argum_count: the argument count
 *@line_count: the error count
 *@error_code: the error code for exit()s
 *@line_count: if on count this line of input
 *@file_name: the program filename
 *@env_copy: linked list local copy of environ
 *@hist_node: the history node
 *@alias_node: the alias node
 *@environ: custom modified copy of environ from LL env
 *@changed: on if environ was changed
 *@status: the return status of the last exec'd command
 *@cmd_buf: address of pointer to cmd_buf, on if chaining
 *@cmd_buf_type: CMD_type ||, &&, ;
 *@readfd: the fd from which to read line input
 *@hist_count: the history line number count
 */
typedef struct passinfo
{
	char *argum;
	char **argum_arr;
	char *path_finder;
	int argum_count;
	unsigned int line_count;
	int error_code;
	int line_count;
	char *file_name;
	list_t *env_copy;
	list_t *hist_node;
	list_t *alias_node;
	char **environ;
	int changed;
	int status;

	char **cmd_buf; /* pointer to cmd ; chain buffer, for memory mangement */
	int cmd_buf_type; /* CMD_type ||, &&, ; */
	int readfd;
	int hist_count;
} info_t;

#define INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
	0, 0, 0}

/**
 *struct builtin - contains a builtin string and related function
 *@type: the builtin command flag
 *@func: the function
 */
typedef struct builtin
{
	char *type;
	int (*func)(info_t *);
} builtin_table;

/* shell essentials */
int exit_s(info_t *info);
int _cd_shell(info_t *info);
int _helper(info_t *info);

/** environment builtins */
int new_env(info_t *info);
int env_builts(info_t *info);
char *env_val(info_t *info, const char *norm);
int env_clear(info_t *info);
int adds_env_list(info_t *info);

/* read/write buffers */
#define READ_BUFFER_SIZE 1024
#define WRITE_BUFFER_SIZE 1024
#define BUFFER_FLUSH -1

#endif

