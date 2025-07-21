/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_escape.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenne <jenne@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 16:17:51 by jenne             #+#    #+#             */
/*   Updated: 2025/07/16 16:18:01 by jenne            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*handle_escape_char(char c)
{
	char	*result;

	result = malloc(2);
	if (!result)
		return (NULL);
	if (c == 'n')
		result[0] = '\n';
	else if (c == 't')
		result[0] = '\t';
	else if (c == 'r')
		result[0] = '\r';
	else if (c == 'b')
		result[0] = '\b';
	else if (c == 'f')
		result[0] = '\f';
	else if (c == 'v')
		result[0] = '\v';
	else
		result[0] = c;
	result[1] = '\0';
	return (result);
}

int	append_escaped_char(char *line, int *i, char **result)
{
	char	*temp;
	char	*escaped;

	(*i)++;
	escaped = handle_escape_char(line[*i]);
	if (!escaped)
		return (0);
	temp = *result;
	*result = ft_strjoin(*result, escaped);
	free(temp);
	free(escaped);
	return (*result != NULL);
}

int	append_regular_char(char *line, int *i, char **result)
{
	char	*temp;

	temp = *result;
	*result = ft_strjoin_char(*result, line[*i]);
	free(temp);
	return (*result != NULL);
}

int	handle_quoted_content(char *line, int *i, char **result, char quote)
{
	while (line[*i] && line[*i] != quote)
	{
		if (line[*i] == '\\' && quote == '"' && line[*i + 1])
		{
			if (!append_escaped_char(line, i, result))
				return (0);
		}
		else
		{
			if (!append_regular_char(line, i, result))
				return (0);
		}
		(*i)++;
	}
	if (line[*i] == quote)
		(*i)++;
	return (1);
}

int	handle_quote_in_word(char *line, int *i, char **word)
{
	char	quote_char;

	quote_char = line[*i];
	(*i)++;
	if (!handle_quoted_content(line, i, word, quote_char))
		return (0);
	return (1);
}
