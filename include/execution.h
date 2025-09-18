/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 22:00:09 by mring             #+#    #+#             */
/*   Updated: 2025/09/18 22:00:17 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
void							free_envp(char **envp);

// fork pipeline
void							handle_parent(t_cmd_node *curr,
									t_cmd_node *prev);
void							handle_child(t_cmd_node *curr, t_cmd_node *prev,
									t_env *ms_env, t_cmd_list *cmd_list);
void							wait_children(t_cmd_list *cmd_list);

// redirections
void							handle_input(char *infile);
void							handle_output(char *outfile, bool append);
void							handle_heredoc(t_file_node *file);
void							handle_redirections(t_cmd_node *curr);

// builtin
void							handle_single_builtin(t_cmd_node *curr,
									t_env *ms_env, t_cmd_list *cmd_list);
int								check_builtin(t_cmd_node *curr);
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

void							preprocess_heredocs(t_cmd_list *cmd_list);
void							cleanup_unused_heredocs(t_cmd_list *cmd_list);

#endif