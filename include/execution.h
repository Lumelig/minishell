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

// execution
void							executor(t_cmd_list *cmd_list, t_env *ms_env);
void							exec_cmd(t_cmd_node *curr, t_env *ms_env);
char							*get_exec_path(t_cmd_node *curr, t_env *ms_env);
char							**env_convert(t_env *ms_env);
void							free_envp(char **envp);

// fork pipeline
void							handle_parent(t_cmd_node *curr, t_env *ms_env);
void							handle_child(t_cmd_node *curr, t_env *ms_env);
void							wait_children(pid_t *pid, ssize_t count);

// redirections
//

// builtin
int								run_builtin(t_cmd_node *curr, t_env *ms_env,
									t_cmd_list *cmd_list);
void							pwd_builtin(void);
void							env_builtin(t_cmd_node *curr, t_env *ms_env);
bool							env_key_update(t_env *ms_env, char *key,
									char *new_value);
bool							insert_env_node(char *key, char *value,
									t_env *ms_env);
void							echo_builtin(t_cmd_node *curr);
void							exit_builtin(t_cmd_node *curr, t_env *ms_env,
									t_cmd_list *cmd_list);
void							export_builtin(t_cmd_node *curr, t_env *ms_env);
void							cd_builtin(t_cmd_node *curr, t_env *ms_env);
void							unset_builtin(t_cmd_node *curr, t_env *ms_env);

// here_doc
//

#endif