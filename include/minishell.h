#ifndef MINISHELL_H
# define MINISHELL_H

# include "execution.h"
# include "libft.h"
# include "parsing.h"
# include <dirent.h> // opendir, readdir, closedir
# include <stdio.h>  // printf, perror
// ~~~
# include <fcntl.h>             // open
# include <readline/history.h>  // add_history, rl_clear_history
# include <readline/readline.h> // rl_on_new_line, rl_replace_line, rl_redisplay
# include <signal.h>            // signal, sigaction, sigemptyset, sigaddset,
								// kill
# include <stdbool.h>
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

/* Environment structures */
typedef struct s_envlist // char **env into a list
{
	char *key;
	char *value;
	struct s_envlist *next;
}				t_envlist;

typedef struct s_env // Controling struct for env
{
	pid_t pid; // extra information put here for easy access
	int size;
	int shlvl; // help to handle an edge case
	t_envlist *head;
	t_envlist *tail; // same as for pid above
}				t_env;

/* Global functions */
int				*exit_code(void);
void			setup_signal_handlers(void);

/* Environment management functions */
bool			add_env_var(t_env *env, char *key, char *value);
bool			add_pwd_if_missing(t_env *my_env);
bool			add_shlvl_if_missing(t_env *my_env);
bool			check_required_vars(t_env *my_env);
bool			init_default_env(t_env *my_env);
bool			init_environment(t_env *my_env, char **env, char **argv,
					int argc);
t_token			*tokenize(char *line);
char			*get_complete_input(void);
t_quote_state	check_line_completion(char *line);
int				check_quotes_balanced_enhanced(char *line);
void			expand_tokens(t_token *token, t_envlist *envlist, t_env *env);
int				copy_special_var(char *result, char *str, int *i, t_env *env);
int				copy_variable(char *result, char *str, int *i,
					t_envlist *envlist);
int				get_special_var_skip(char *str, int i);
int				is_special_var(char *str, int pos);
int				is_special_expansion(char *str, int i);
int				calculate_var_size(char *str, int i, t_envlist *envlist,
					t_env *env);
int				calculate_special_var_size(char *str, int i, t_env *env);
int				get_var_length(char *str, int start, int *end_pos);
void			free_environment(t_env *my_env);
t_cmd_list		*token_to_cmd(t_token *token);
void			print_cmd_list(t_cmd_list *cmd_list);

void			init_empty_env(t_env *env, char **argv, int argc);
void			set_environment(t_env *my_env, char **key, char **value,
					char *env);
bool			init_default_env(t_env *my_env);
bool			process_env_vars(t_env *my_env, char **env);
bool			update_or_add_env_var(t_env *env, char *key, char *value);
char			*create_env_string(t_envlist *current);
char			*get_env_value(t_env *env, const char *key);
char			**env_to_array(t_env *env);
t_envlist		*find_env_var(t_env *env, const char *key);
void			cleanup_env(t_env *env);
void			free_env_array(char **env_array, int count);
char			**env_to_array(t_env *env);

#endif