#ifndef MINISHELL_H
# define MINISHELL_H

//# include "execution.h"
# include "libft.h"
# include <dirent.h> // opendir, readdir, closedir
# include <fcntl.h>  // open
# include <signal.h> // signal, sigaction, sigemptyset, sigaddset,
// kill
# include <stdio.h> // printf, perror
//~~~
# include <readline/history.h>  // add_history, rl_clear_history
# include <readline/readline.h> // readline, rl_on_new_line, rl_replace_line,
// rl_redisplay
# include <stdbool.h>
# include <stdlib.h>       // getenv
# include <stdlib.h>       // malloc, free, exit
# include <string.h>       // strerror
# include <sys/ioctl.h>    // ioctl
# include <sys/resource.h> // struct rusage (used in wait3/wait4)
# include <sys/stat.h>     // stat, lstat, fstat
# include <sys/time.h>     // struct timeval (used in wait3/wait4)
# include <sys/types.h>    // types for stat, wait
# include <sys/wait.h>     // wait, waitpid, wait3, wait4
# include <term.h>         // tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs
# include <termios.h>      // tcgetattr, tcsetattr
# include <unistd.h>       // write, access, fork, getcwd, chdir, dup, dup2,
							// pipe, isatty, ttyname, ttyslot

typedef enum
{
	TOKEN_WORD,         // e.g., echo, hello
	TOKEN_PIPE,         // |
	TOKEN_REDIR_IN,     // <
	TOKEN_REDIR_OUT,    // >
	TOKEN_REDIR_APPEND, // >>
	TOKEN_HEREDOC,      // <<
	TOKEN_EOF
}							t_token_type;

typedef struct s_token
{
	t_token_type			type;
	char					*value;
	struct s_token			*next;
}							t_token;

typedef struct s_file_node	t_file_node;
// typedef struct s_cmd_node	t_cmd_node;
typedef sig_atomic_t		sig_atomic_t;

typedef struct s_file_list
{
	t_file_node				*head;
	t_file_node				*tail;
	ssize_t					size;
}							t_file_list;

typedef struct s_file_node
{
	char					*filename;
	int						redir_type;
	struct s_file_node		*next;
}							t_file_node;

typedef struct s_cmd_node
{
	int						cmd_type;
	char					**cmd;
	t_file_list				*files;
	struct s_cmd_node		*next;
}							t_cmd_node;
typedef struct s_cmd_list
{
	t_cmd_node				*head;
	t_cmd_node				*tail;
	ssize_t					size;
}							t_cmd_list;


typedef struct s_quote_state
{
	int						in_single;
	int						in_double;
	int						continuation;
}							t_quote_state;

typedef struct s_envlist // char **env into a list
{
	char *key;
	char delimiter;
	char *value;
	struct s_envlist *next;
}							t_envlist;

typedef struct s_env // Controling struct for env
{
	pid_t pid; // extra information put here for easy access
	int size;
	int shlvl; // help to handle an edge case
	t_envlist *head;
	t_envlist *tail;
	int exit_status; // same as for pid above
}							t_env;

char	*get_continuation_prompt(int quote_type);

t_cmd_list					*parsing(t_env *my_env, t_token *token);

void						setup_signal_handlers(void);

bool						init_environment(t_env *my_env, char **env,
								char **argv, int argc);

t_token						*tokenize(char *line);

char						*get_complete_input(void);

t_quote_state				check_line_completion(char *line);

int							check_quotes_balanced_enhanced(char *line);

int							check_quotes_balanced(char *line);

void						expand_tokens(t_token *token, t_envlist *envlist,
								t_env *env);

void						copy_special_var(char *result, int *j, char *str,
								int *i, t_env *env);

void						copy_variable(char *result, int *j, char *str,
								int *i, t_envlist *envlist);

int							get_special_var_skip(char *str, int i);

int							is_special_var(char *str, int pos);

int							is_special_expansion(char *str, int i);

int							calculate_var_size(char *str, int i,
								t_envlist *envlist, t_env *env);

int							calculate_special_var_size(char *str, int i,
								t_env *env);

int							get_var_length(char *str, int start, int *end_pos);

void						free_environment(t_env *my_env);

t_cmd_list					*token_to_cmd(t_token *token);
void print_cmd_list(t_cmd_list *cmd_list);

#endif