#ifndef CUSTOM_SHELL_H
#define CUSTOM_SHELL_H

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
#define READ_BUFFER_SIZE 1024
#define WRITE_BUFFER_SIZE 1024
#define BUFFER_FLUSH -1

/* for command chaining */
#define CMD_NORMAL 0
#define CMD_OR 1
#define CMD_AND 2
#define CMD_CHAIN 3

/* for converting numbers */
#define CONVERT_LOWERCASE 1
#define CONVERT_UNSIGNED 2

/* Using system getline */
#define USE_GETLINE 0
#define USE_STRTOK 0

#define HISTORY_FILE ".custom_shell_history"
#define MAX_HISTORY_ENTRIES 4096

extern char **custom_environ;

/**
 * struct linked_string - singly linked list
 * @number: the number field
 * @string: a string
 * @next: points to the next node
 */
typedef struct linked_string
{
	int number;
	char *string;
	struct linked_string *next;
} string_list_t;

/**
 * struct command_info - contain arguments to pass into function,
 * allowing uniform prototype for function pointer struct
 * @arg: a string generated from getline contains arguments
 * @argv: array of strings generated from arg
 * @path: string path for current command
 * @argc: argument count
 * @line_count: error count
 * @err_num: error code for exit()s
 * @linecount_flag: if on, count this line of input
 * @filename: program filename
 * @environment_list: linked list local copy of environ
 * @custom_environment: custom modified copy of environ from LL environment_list
 * @history: the history node
 * @alias: the alias node
 * @environment_changed: on if environ was changed
 * @command_status: the return status of the last exec'd command
 * @command_buffer: address of pointer to command_buffer, on if chaining
 * @command_buffer_type: CMD_type ||, &&, ;
 * @read_descriptor: the file descriptor from which to read line input
 * @history_count: the history line number count
 */
typedef struct command_info
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int line_count;
	int err_num;
	int linecount_flag;
	char *filename;
	string_list_t *environment_list;
	string_list_t *history;
	string_list_t *alias;
	char **custom_environment;
	int environment_changed;
	int command_status;

	char **command_buffer; /* pointer to command ; chain buffer, for memory management */
	int command_buffer_type; /* CMD_type ||, &&, ; */
	int read_descriptor;
	int history_count;
} info_t;

#define INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
		0, 0, 0}

/**
 * struct custom_builtin - contains a builtin string and related function
 * @type: the builtin command flag
 * @func: the function
 */
typedef struct custom_builtin
{
	char *type;
	int (*func)(info_t *);
} builtin_table;


/* custom_shell_loop.c */
int custom_shell(info_t *, char **);
int find_builtin_command(info_t *);
void find_command(info_t *);
void fork_command(info_t *);

/* custom_parser.c */
int is_command(info_t *, char *);
char *duplicate_characters(char *, int, int);
char *find_command_path(info_t *, char *, char *);

/* loop_custom_shell.c */
int loop_custom_shell(char **);

/* custom_errors.c */
void custom_error_puts(char *);
int custom_error_putchar(char);
int custom_putchar_fd(char c, int fd);
int custom_puts_fd(char *str, int fd);

/* custom_string.c */
int custom_strlen(char *);
int custom_strcmp(char *, char *);
char *starts_with(const char *, const char *);
char *custom_strcat(char *, char *);

/* custom_string1.c */
char *custom_strcpy(char *, char *);
char *custom_strdup(const char *);
void custom_puts(char *);
int custom_putchar(char);

/* custom_exits.c */
char *custom_strncpy(char *, char *, int);
char *custom_strncat(char *, char *, int);
char *custom_strchr(char *, char);

/* custom_tokenizer.c */
char **custom_strtow(char *, char *);
char **custom_strtow2(char *, char);

/* custom_realloc.c */
char *custom_memset(char *, char, unsigned int);
void free_custom(char **);
void *custom_realloc(void *, unsigned int, unsigned int);

/* custom_memory.c */
int custom_bfree(void **);

/* custom_atoi.c */
int check_interactive(info_t *);
int check_delimiter(char, char *);
int check_alphabetic(int);
int custom_atoi(char *);

/* custom_errors1.c */
int custom_erratoi(char *);
void print_custom_error(info_t *, char *);
int print_d(int, int);
char *convert_custom_number(long int, int, int);
void remove_comments(char *);

/* custom_builtin.c */
int custom_exit(info_t *);
int custom_change_directory(info_t *);
int custom_help(info_t *);

/* custom_builtin1.c */
int custom_history(info_t *);
int custom_alias(info_t *);

/* custom_getline.c */
ssize_t get_custom_input(info_t *);
int custom_getline(info_t *, char **, size_t *);
void sigint_handler(int);

/* custom_getinfo.c */
void clear_custom_info(info_t *);
void set_custom_info(info_t *, char **);
void free_custom_info(info_t *, int);

/* custom_environ.c */
char *custom_getenv(info_t *, const char *);
int custom_show_environment(info_t *);
int custom_set_environment(info_t *);
int custom_unset_environment(info_t *);
int populate_environment_list(info_t *);

/* custom_getenv.c */
char **get_custom_environment(info_t *);
int custom_unsetenv(info_t *, char *);
int custom_setenv(info_t *, char *, char *);

/* custom_history.c */
char *get_history_file(info_t *info);
int write_custom_history(info_t *info);
int read_custom_history(info_t *info);
int build_custom_history_list(info_t *info, char *buffer, int linecount);
int renumber_custom_history(info_t *info);

/* custom_lists.c */
string_list_t *add_string_node(string_list_t **, const char *, int);
string_list_t *add_string_node_end(string_list_t **, const char *, int);
size_t print_string_list(const string_list_t *);
int delete_string_node_at_index(string_list_t **, unsigned int);
void free_string_list(string_list_t **);

/* custom_lists1.c */
size_t string_list_length(const string_list_t *);
char **list_to_string_array(string_list_t *);
size_t print_list(const string_list_t *);
string_list_t *string_node_starts_with(string_list_t *, char *, char);
ssize_t get_node_index(string_list_t *, string_list_t *);

/* custom_vars.c */
int check_command_chain(info_t *, char *, size_t *);
void handle_command_chain(info_t *, char *, size_t *, size_t, size_t);
int replace_alias(info_t *);
int replace_custom_variables(info_t *);
int replace_string(char **, char *);

#endif
