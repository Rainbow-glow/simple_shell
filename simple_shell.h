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
 *@line_count_flag: if on count this line of input
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
	int line_count_flag;
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

/* shell token */
char **str_to_wrds(char *str, char *d);
char **str_to_words_del(char *str, char d);

/* shell checkers */
int inter_active(info_t *info);
int _isdel(char character, char *del);
int is_alpha(int c);
int str_to_int(char *str);
int is_chain(info_t *info, char *buf, size_t *p);
void check_chain(info_t *info, char *buf, size_t *p, size_t i, size_t len);
int replace_alias(info_t *info);
int replace_vars(info_t *info);
int replace_string(char **old, char *new);

/* shell strings */
int StrLen(char *str);
int Strcmp(char *str1, char *str2);
char *start_str(const char *cobas, const char *mindray);
char *StrCat(char *dest, char *src);
char StrCpy(char *dest, char *src);
char *StrDup(const char *);
void _write(char *);
int put_char(char c);
char *StrnCpy(char *dest, char *src, int index);
char *StrnCat(char *dest, char *src, int index);
char *StrChr(char *s, char c);

/* shell essentials */
int main(int ac, char **av);
int exit_me(info_t *info);
int cd_me(info_t *info);
int helper_me(info_t *info);
int myhistori(info_t *info);
int remv_alias(info_t *info, char *string);
int init_alias(info_t *info, char *string);
int puts_alias(list_t *node);
int alias_mem(info_t *info);
int loop_shell(info_t *info, char **av);
int find_builtin(info_t *info);
void find_command(info_t *info);
void fork_command(info_t *info);
void clear_info(info_t *info);
void set_info(info_t *info, char **av);
void free_info(info_t *info, int all);

/* shell memory */
char *_memset(char *s, char b, unsigned int n);
void ffree(char **pp);
void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size);
int bfree(void **ptr);

/* shell errors */
void _eputs(char *str);
int _eputchar(char c);
int _putfd(char c, int fd);
int _putsfd(char *str, int fd);
int _erratoi(char *s);
void print_error(info_t *info, char *estr);
int print_d(int input, int fd);
char *convert_number(long int num, int base, int flags);
void remove_comments(char *buf);

/* shell history */
char *get_history_file(info_t *info);
int write_history(info_t *info);
int read_history(info_t *info);
int build_history_list(info_t *info, char *buf, int line_count);
int renumber_history(info_t *info);

/* shell list */
list_t *add_node(list_t **head, const char *string, int find_num);
list_t *add_node_end(list_t **head, const char *string, int find_num);
int delete_node_at_index(list_t **head, unsigned int index);
size_t print_list_str(const list_t *h);
void free_list(list_t **head_ptr);
size_t list_len(const list_t *h);
char **list_to_strings(list_t *head);
size_t print_list(const list_t *h);
list_t *node_starts_with(list_t *node, char *prefix, char c);
ssize_t get_node_index(list_t *head, list_t *node);

/** environment builtins */
char **get_env(info_t *info);
int del_env(info_t *info, char *var);
int set_env(info_t *info, char *var, char *vad);
int pwd_env(info_t *info);
char *gets_env(info_t *info, const char *name);
int init_env(info_t *info);
int remv_env(info_t *info);
int add_env_list(info_t *info);

/* shell get line */
ssize_t takes_buf(info_t *info, char **buf, size_t *len);
ssize_t get_new_buf(info_t *info);
ssize_t buf_reader(info_t *info, char *buf, size_t *i);
int get_line(info_t *info, char **ptr, size_t *length);
void C_Handler(__attribute__((unused))int sig_num);

/* shell parser */
int exe_c(info_t *info, char *path);
char *dupli_c(char *pathstr, int start, int stop);
char *find_pathstr(info_t *info, char *pathstr, char *cmd);

/* read/write buffers */
#define READ_BUFFER_SIZE 1024
#define WRITE_BUFFER_SIZE 1024
#define BUFFER_FLUSH -1

#endif
