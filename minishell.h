#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdlib.h>      // getenv
#include "libft/libft.h"
# include <stdbool.h>

#include <stdio.h>       // printf, perror
#include <stdlib.h>      // malloc, free, exit
#include <string.h>      // strerror
#include <unistd.h>      // write, access, fork, getcwd, chdir, dup, dup2, pipe, isatty, ttyname, ttyslot

#include <fcntl.h>       // open
#include <sys/types.h>   // types for stat, wait
#include <sys/stat.h>    // stat, lstat, fstat
#include <dirent.h>      // opendir, readdir, closedir

#include <sys/wait.h>    // wait, waitpid, wait3, wait4
#include <sys/time.h>    // struct timeval (used in wait3/wait4)
#include <sys/resource.h> // struct rusage (used in wait3/wait4)

#include <signal.h>      // signal, sigaction, sigemptyset, sigaddset, kill

#include <termios.h>     // tcgetattr, tcsetattr
#include <sys/ioctl.h>   // ioctl

#include <term.h>        // tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs

#include <readline/readline.h>      // readline, rl_on_new_line, rl_replace_line, rl_redisplay
#include <readline/history.h>       // add_history, rl_clear_history

typedef enum {
    TOKEN_WORD,        // e.g., echo, hello
    TOKEN_PIPE,        // |
    TOKEN_REDIR_IN,    // <
    TOKEN_REDIR_OUT,   // >
    TOKEN_REDIR_APPEND,// >>
    TOKEN_HEREDOC,     // <<
    TOKEN_EOF
} t_token_type;

typedef struct s_token {
    t_token_type type;
    char *value;
    struct s_token *next;
} t_token;


typedef struct s_file_node t_file_node;
typedef struct s_cmd_node t_cmd_node;

typedef struct s_file_list
{
	t_file_node	*head;
	t_file_node	*tail;
	ssize_t		  size;
}	t_file_list;

typedef struct s_cmd_list
{
	t_cmd_node	*head;
	t_cmd_node	*tail;
	ssize_t		  size;
}	t_cmd_list;

typedef struct s_file_node
{
	char				        *filename;
	int					        redir_type;
	struct s_file_node	*next;
}	t_file_node;

typedef struct s_cmd_node
{
	int					      cmd_type;
	char				      **cmd;
	t_file_list			  *files;
	struct s_cmd_node	*next;
}	t_cmd_node;

typedef struct s_quote_state {
    int in_single_quote;
    int in_double_quote;
    int needs_continuation;
} t_quote_state;
t_token *tokenize(char *line);

char *get_complete_input(void);

t_quote_state check_line_completion(char *line);

int check_quotes_balanced_enhanced(char *line);

int check_quotes_balanced(char *line);

#endif