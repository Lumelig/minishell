/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 01:30:11 by jenne             #+#    #+#             */
/*   Updated: 2025/09/19 19:50:42 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_redirection(t_token_type type)
{
	return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_REDIR_APPEND || type == TOKEN_HEREDOC);
}

static char	*get_token(t_token_type type)
{
	if (type == TOKEN_PIPE)
		return ("`|'");
	else if (type == TOKEN_REDIR_IN)
		return ("`<'");
	else if (type == TOKEN_REDIR_OUT)
		return ("`>'");
	else if (type == TOKEN_REDIR_APPEND)
		return ("`>>'");
	else if (type == TOKEN_HEREDOC)
		return ("`<<'");
	else
		return ("unexpected token");
}

static void	print_syntax_error(char *token_str)
{
	write(STDERR_FILENO, "minishell: syntax error near unexpected token ", 46);
	write(STDERR_FILENO, token_str, ft_strlen(token_str));
	write(STDERR_FILENO, "\n", 1);
}

bool	syntax_validation(t_token *token)
{
	t_token	*current;
	t_token	*previous;

	current = token;
	previous = NULL;
	while (current && current->type != TOKEN_EOF)
	{
		if ((!previous && current->type == TOKEN_PIPE) || (previous
				&& previous->type == TOKEN_PIPE && (current->type == TOKEN_PIPE
					|| current->type == TOKEN_EOF)))
			return (print_syntax_error("`|'"), false);
		if (is_redirection(current->type))
		{
			if (!current->next)
				return (print_syntax_error("`newline'"), false);
			if (current->next->type != TOKEN_WORD)
				return (print_syntax_error(get_token(current->next->type)),
					false);
		}
		previous = current;
		current = current->next;
	}
	if (previous && previous->type == TOKEN_PIPE)
		return (print_syntax_error("`|'"), false);
	return (true);
}

t_cmd_list	*parsing(t_env *my_env, t_token *token)
{
	t_cmd_list	*cmd_list;

	cmd_list = NULL;
	expand_tokens(token, my_env->head, my_env);
	if (!syntax_validation(token))
		*exit_code() = 2;
	else
	{
		*exit_code() = 0;
		cmd_list = token_to_cmd(token);
	}
	return (cmd_list);
}
