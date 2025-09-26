/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_escape.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpflegha <jpflegha@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 16:17:51 by jenne             #+#    #+#             */
/*   Updated: 2025/09/26 14:35:52 by jpflegha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	append_escaped_char(char *line, int *i, char **result)
{
	char	*temp;
	char	char_to_add[3];

	(*i)++;
	if (line[*i] == '"' || line[*i] == '\\' || line[*i] == '$')
	{
		char_to_add[0] = line[*i];
		char_to_add[1] = '\0';
	}
	else
	{
		char_to_add[0] = '\\';
		char_to_add[1] = line[*i];
		char_to_add[2] = '\0';
	}
	temp = *result;
	*result = ft_strjoin(*result, char_to_add);
	free(temp);
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
	if(line[*i + 1] == quote_char && line[*i + 2] == ' ')
	{
		*word = ft_strdup("");
	}
	(*i)++;
	if (!handle_quoted_content(line, i, word, quote_char))
		return (0);
	return (1);
}
