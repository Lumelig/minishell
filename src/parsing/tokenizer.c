/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpflegha <jpflegha@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 16:07:45 by jenne             #+#    #+#             */
/*   Updated: 2025/08/30 15:07:47 by jpflegha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_other_operator(int *i, t_token **head, t_token_type type, char c)
{
	char	str[2];

	str[0] = c;
	str[1] = '\0';
	if (!add_token(head, type, str, QUOTE_NONE))
		return (0);
	(*i)++;
	return (1);
}

int	handle_output_redirect(char *line, int *i, t_token **head)
{
	if (line[*i + 1] && line[*i + 1] == '>')
	{
		if (!add_token(head, TOKEN_REDIR_APPEND, ">>", QUOTE_NONE))
			return (0);
		(*i) += 2;
	}
	else
	{
		if (!add_token(head, TOKEN_REDIR_OUT, ">", QUOTE_NONE))
			return (0);
		(*i)++;
	}
	return (1);
}

int	handle_input_redirect(char *line, int *i, t_token **head)
{
	if (line[*i + 1] && line[*i + 1] == '<')
	{
		if (!add_token(head, TOKEN_HEREDOC, "<<", QUOTE_NONE))
			return (0);
		(*i) += 2;
	}
	else
	{
		if (!add_token(head, TOKEN_REDIR_IN, "<", QUOTE_NONE))
			return (0);
		(*i)++;
	}
	return (1);
}

int	handle_operator(char *line, int *i, t_token **head)
{
	if (line[*i] == '|')
		return (handle_other_operator(i, head, TOKEN_PIPE, '|'));
	else if (line[*i] == '>')
		return (handle_output_redirect(line, i, head));
	else if (line[*i] == '<')
		return (handle_input_redirect(line, i, head));
	return (1);
}

t_token	*tokenize(char *line)
{
	t_token	*head;
	int		i;

	head = NULL;
	i = 0;
	if (!line)
		return (NULL);
	while (line[i])
	{
		if (ft_isspace(line[i]) && line[i])
		{
			i++;
		}
		else if (is_operator_char(line[i]))
		{
			if (!handle_operator(line, &i, &head))
				return (cleanup_tokens(head));
		}
		else if (!process_word_token(line, &i, &head))
			return (cleanup_tokens(head));
	}
	if (!add_token(&head, TOKEN_EOF, "", QUOTE_NONE))
		return (cleanup_tokens(head));
	return (head);
}

// t_token	*tokenize(char *line)
// {
// 	t_token	*head;
// 	int		i;
//
// 	head = NULL;
// 	i = 0;
// 	if (!line)
// 		return (NULL);
// 	while (line[i])
// 	{
// 		if (line[i] == '\n')
// 		{
// 			if (!handle_operator(line, &i, &head))
// 				return (cleanup_tokens(head));
// 		}
// 		if (ft_isspace(line[i]))
// 		{
// 			i++;
// 			continue ;
// 		}
// 		if (is_operator_char(line[i]) && !handle_operator(line, &i, &head))
// 			return (cleanup_tokens(head));
// 		else if (!is_operator_char(line[i])
// 			&& !process_word_token(line, &i, &head))
// 			return (cleanup_tokens(head));
// 	}
// 	if (!add_token(&head, TOKEN_EOF, "", QUOTE_NONE))
// 		return (cleanup_tokens(head));
// 	return (head);
// }
