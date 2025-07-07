#ifndef EXECUTION_H
# define EXECUTION_H

typedef struct s_token			t_token;
typedef struct s_file_node		t_file_node;
typedef struct s_file_list		t_file_list;
typedef struct s_cmd_node		t_cmd_node;
typedef struct s_cmd_list		t_cmd_list;
typedef struct s_quote_state	t_quote_state;
typedef struct s_envlist		t_envlist;
typedef struct s_env			t_env;

// debugging
void							create_cmd(int cmd_type, char **args,
									t_cmd_list *cmd_list);
void							debug_test(t_cmd_list *cmd_list, t_env *env);

// exec
int								executor(t_cmd_list *cmd_list, t_env *env);

// builtin
int								builtin_check(t_cmd_node *cmd);
void							run_builtin(int id, t_cmd_node *curr,
									t_env *env);
void							cd_builtin(void);
void							echo_builtin(void);
void							exit_builtin(void);
void							env_builtin(void);
void							export_builtin(void);
void							unset_builtin(void);
void							pwd_builtin(void);

#endif