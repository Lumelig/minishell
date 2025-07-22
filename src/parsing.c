/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenne <jenne@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 01:30:11 by jenne             #+#    #+#             */
/*   Updated: 2025/07/22 13:00:42 by jenne            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_operator(t_token *token)
{
	if (!token)
		return (0);
	return (token->type == TOKEN_PIPE || token->type == TOKEN_REDIR_APPEND
		|| token->type == TOKEN_REDIR_OUT || token->type == TOKEN_REDIR_IN);
}

int	syntax_validation(t_token *token)
{
	t_token	*current;
	t_token	*preveious;

	current = token;
	preveious = NULL;
	while (current && current->type != TOKEN_EOF)
	{
		if (!preveious && current->type == TOKEN_PIPE)
		{
			printf("minishell: syntax error near unexpected token '|'\n");
			return (2);
		}
		preveious = current;
		current = current->next;
	}
	return (0);
}

t_cmd_list	*parsing(t_env *my_env, t_token *token)
{
	t_cmd_list	*cmd_list;
	static int exit;

	expand_tokens(token, my_env->head, my_env);
	exit = syntax_validation(token);
	if (exit > 0)
		*exit_code() = 2;
	cmd_list = token_to_cmd(token);
	return (cmd_list);
}
