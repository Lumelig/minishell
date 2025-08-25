/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenne <jenne@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 13:46:17 by jenne             #+#    #+#             */
/*   Updated: 2025/08/25 19:00:39 by jenne            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "libft.h"
# include <stdbool.h>
# include <stdlib.h>
# include <sys/types.h>

/* Token definitions */
typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_HEREDOC,
	TOKEN_END_CMD,
	TOKEN_EOF
}							t_token_type;

typedef enum e_qoute
{
	QUOTE_NONE,
	QUOTE_SINGLE,
	QUOTE_DOUBLE,
	QUOTE_MIXED
}							t_quote;
typedef struct s_token
{
	t_token_type			type;
	char					*value;
	t_quote					qoute;
	struct s_token			*next;
}							t_token;

/* File and command structures */
typedef struct s_file_node
{
	char					*filename;
	int						redir_type;
	struct s_file_node		*next;
}							t_file_node;

typedef struct s_file_list
{
	t_file_node				*head;
	t_file_node				*tail;
	ssize_t					size;
}							t_file_list;


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

/* Quote handling */
typedef struct s_quote_state
{
	int						in_single;
	int						in_double;
	int						continuation;
}							t_quote_state;

/* Core parsing functions */
t_cmd_list					*parsing(t_env *my_env, t_token *token);
t_cmd_list					*token_to_cmd(t_token *token);
t_token						*tokenize(char *line);

/* Input processing */
char						*get_complete_input(void);
char						*get_continuation_prompt(int quote_type);
t_quote_state				check_line_completion(char *line);
int							check_quotes_balanced_enhanced(char *line);
void						process_quote_state(char *line, int *i,
								t_quote_state *state);

/* Token expansion */
void						expand_tokens(t_token *token, t_envlist *envlist,
								t_env *env);
int							calculate_special_var_size(char *str, int i,
								t_env *env);
int							calculate_var_size(char *str, int i,
								t_envlist *envlist, t_env *env);
int							copy_special_var(char *result, char *str, int *i,
								t_env *env);
int							copy_variable(char *result, char *str, int *i,
								t_envlist *envlist);
int							get_special_var_skip(char *str, int i);
int							get_var_length(char *str, int start, int *end_pos);
int							is_special_expansion(char *str, int i);
int							is_special_var(char *str, int pos);

/* Environment management */
bool						add_env_var(t_env *env, char *key, char *value);
bool						add_pwd_if_missing(t_env *my_env);
bool						add_shlvl_if_missing(t_env *my_env);
bool						check_required_vars(t_env *my_env);
bool						init_default_env(t_env *my_env);
bool						init_environment(t_env *my_env, char **env,
								char **argv, int argc);
bool						process_env_vars(t_env *my_env, char **env);
bool						update_or_add_env_var(t_env *env, char *key,
								char *value);
char						*create_env_string(t_envlist *current);
char						*get_env_value(t_env *env, const char *key);
char						**env_to_array(t_env *env);
t_envlist					*find_env_var(t_env *env, const char *key);
void						cleanup_env(t_env *env);
void						free_env_array(char **env_array, int count);
void						free_environment(t_env *my_env);
void						init_empty_env(t_env *env, char **argv, int argc);
void						set_environment(t_env *my_env, char **key,
								char **value, char *env);

/* Tokenizer functions */
char						*handle_escape_char(char c);
int							add_token(t_token **head, t_token_type type,
								char *value, t_quote qoute);
int							append_escaped_char(char *line, int *i,
								char **result);
int							append_regular_char(char *line, int *i,
								char **result);
int							extract_word(char *line, int *i, char **word,
								t_quote *quote);
int							handle_escape_in_word(char *line, int *i,
								char **word);
int							handle_input_redirect(char *line, int *i,
								t_token **head);
int							handle_operator(char *line, int *i, t_token **head);
int							handle_output_redirect(char *line, int *i,
								t_token **head);
int							handle_pipe_operator(int *i, t_token **head);
int							handle_quoted_content(char *line, int *i,
								char **result, char quote);
int							handle_quote_in_word(char *line, int *i,
								char **word);
int							handle_regular_char_in_word(char *line, int *i,
								char **word);
int							is_operator_char(char c);
int							process_word_token(char *line, int *i,
								t_token **head);
t_token						*cleanup_tokens(t_token *head);
t_token						*create_new_token(t_token_type type,
								const char *value, t_quote qoute);
t_token						*find_last_token(t_token *head);

/* Token  to cmd*/
t_cmd_list					*init_cmd_list(void);
t_file_list					*init_file_list(void);
t_file_node					*create_file_node(char *filename,
								t_token_type redir_type);
/* Debug functions */
void						print_cmd_list(t_cmd_list *cmd_list);

#endif