/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenne <jenne@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 16:18:08 by jenne             #+#    #+#             */
/*   Updated: 2025/07/16 16:18:10 by jenne            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*cleanup_tokens(t_token *head)
{
	t_token	*current;
	t_token	*next;

	current = head;
	while (current)
	{
		next = current->next;
		free(current->value);
		free(current);
		current = next;
	}
	return (NULL);
}

t_token	*create_new_token(t_token_type type, const char *value)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->type = type;
	new_token->value = ft_strdup(value);
	if (!new_token->value)
	{
		free(new_token);
		return (NULL);
	}
	new_token->next = NULL;
	return (new_token);
}

t_token	*find_last_token(t_token *head)
{
	t_token	*current;

	current = head;
	while (current->next)
		current = current->next;
	return (current);
}

int	add_token(t_token **head, t_token_type type, const char *value)
{
	t_token	*new_token;
	t_token	*last;

	new_token = create_new_token(type, value);
	if (!new_token)
		return (0);
	if (!*head)
		*head = new_token;
	else
	{
		last = find_last_token(*head);
		last->next = new_token;
	}
	return (1);
}

int	is_operator_char(char c)
{
	return (c == '|' || c == '<' || c == '>');
}
